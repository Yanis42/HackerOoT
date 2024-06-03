#!/usr/bin/env python3

# usage: python3 new_sfx.py --soundfont 0 path/to/my_new_sample.wav
# this won't add a new entry to the tables under include/tables/sfx
# the IDs will use an uppercase version of the .wav's filename
# the sample must be using mono and 16-bits PCM encoding
# paths starting with ``mod_assets/`` are supported
#
# made from Tharo's documentation: https://github.com/Thar0/oot/blob/audio-assets/docs/audio/Custom_Audio_Notes.md

import os
import argparse

from xml.etree import ElementTree as ET
from xml.dom import minidom as MD


INDENT = " " * 4


def get_xml_root(filename: str):
    try:
        root = ET.parse(filename).getroot()
    except FileNotFoundError:
        raise FileNotFoundError(f"ERROR: file '{filename}' was not found.")

    return root


def is_elem_existing(root: ET.Element, attr: str, value: str):
    for elem in root:
        result = elem.get(attr)
        if result is not None and result == value:
            return True
    return False


def write_new_xml(filename: str, root: ET.Element):
    xmlStr = MD.parseString(ET.tostring(root)).toprettyxml(indent="    ", encoding='UTF-8')
    xmlStr = b"\n".join(line for i, line in enumerate(xmlStr.splitlines()) if i > 0)  # remove the first line
    xmlStr = b'\n'.join([s for s in xmlStr.splitlines() if s.strip()])
    with open(filename, "bw") as file:
        file.write(xmlStr + b"\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Helper script to add a new sfx")
    parser.add_argument("-s", "--soundfont", help="number from 0 to 6, soundfont id to use (soundfonts 0 and 1 both use sample bank 0, use 1 if the effect is in the enemy bank)")
    parser.add_argument("path", help="path to the wav file (can be mod_assets)")

    args = parser.parse_args()
    path: str = args.path
    path = path.replace("mod_assets/", "assets/")
    soundfont: int = int(args.soundfont, 0)

    if soundfont > 1:
        raise ValueError(f"ERROR: unsupported soundfont id ('{soundfont}')")

    if not os.path.isfile(args.path):
        raise FileExistsError(f"ERROR: file '{args.path}' doesn't exist!")

    if " " in path:
        raise ValueError("ERROR: the path must not contain any space character!")

    ### Add the wav to Samplebank 0 ###

    bank_id = 0 if soundfont < 0 or soundfont > 6 or soundfont == 1 else soundfont
    bank_path = f"assets/audio/samplebanks/SampleBank_{bank_id}.xml"
    sample_name = path.split("/")[-1].removesuffix(".wav")

    with open(bank_path, "r") as file:
        if f"{sample_name}.aifc" in file.read():
            raise ValueError(f"ERROR: sample '{sample_name}' already exists!")

    # no money :(
    bank_root = get_xml_root(bank_path)

    sample_id = f"SAMPLE_{bank_id}_{sample_name.upper()}"
    ET.SubElement(
        bank_root, 
        "Sample",
        { 
            "Name": sample_id, 
            "Path": f"$(BUILD_DIR)/assets/audio/samples/SampleBank_{bank_id}/{sample_name}.aifc"
        }
    )

    write_new_xml(bank_path, bank_root)

    ### Register the sample as an Effect in either Soundfont 0 or 1 ###

    soundfont_path = f"assets/audio/soundfonts/Soundfont_{soundfont}.xml"
    soundfont_root = get_xml_root(soundfont_path)
    soundfont_samples = soundfont_root.find("Samples")
    soundfont_effects = soundfont_root.find("Effects")

    # add sample to the soundfont

    if soundfont_samples is None:
        raise ValueError("ERROR: 'Samples' node can't be found!")

    if is_elem_existing(soundfont_samples, "Name", sample_id):
        raise ValueError(f"ERROR: sample id '{sample_id}' already exists!")

    ET.SubElement(soundfont_samples, "Sample", {"Name": sample_id})

    # add the effect

    effect_id = f"EFFECT_{sample_name.upper()}"
    effect_len = 0

    if soundfont_effects is None:
        # add the effects list if it's not present yet
        soundfont_effects = ET.SubElement(soundfont_root, "Effects")
    elif is_elem_existing(soundfont_effects, "Name", effect_id):
        # else make sure the new effect doesn't already exist
        raise ValueError(f"ERROR: effect '{effect_id}' already exists!")
    else:
        # get existing effect list length
        effect_len = len(soundfont_effects)

    ET.SubElement(soundfont_effects, "Effect", {"Name": effect_id, "Sample": sample_id})
    write_new_xml(soundfont_path, soundfont_root)

    ### Finally, add the assembly routine ###

    channel = f"CHANNEL_{sample_name.upper()}"
    layer = f"LAYER_{sample_name.upper()}"

    channel_data = (
        f".channel {channel}\n"
        + (INDENT + f"ldlayer     0, {layer}\n")
        + (INDENT + "end\n")
    )

    if effect_len > 64:
        transpose = 0
        while effect_len > 64:
            transpose += 1
            effect_len -= 64

        layer_data = (
            f".layer {layer}\n"
            + (INDENT + f"transpose   {transpose}\n")
            + (INDENT + f"instr       FONTANY_INSTR_SFX\n")
            + (INDENT + f"notedv      (SF{soundfont}_{effect_id} - {transpose} * 64), 0, 100\n")
            + (INDENT + "end\n")
        )
    else:
        layer_data = (
            f".layer {layer}\n"
            + (INDENT + f"instr       FONTANY_INSTR_SFX\n")
            + (INDENT + f"notedv      SF{soundfont}_{effect_id}, 0, 100\n")
            + (INDENT + "end\n")
        )

    # this is what makes other soundfounts ids unsupported
    with open("assets/audio/sequences/seq_0.prg.seq", "a") as file:
        file.write("\n" + channel_data + "\n" + layer_data)

    print(f"Done! Your sample's channel ID is '{channel}'")
