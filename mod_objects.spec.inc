beginseg
    name "object_rat"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_skel.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_attack.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_damaged.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_death.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_endrun.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_idle.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_notice.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_run.o"
    include "$(BUILD_DIR)/assets/objects/object_rat/rat_anim_startrun.o"
    number 6
endseg

beginseg
    name "object_metal_crate"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_metal_crate/object_metal_crate.o"
    number 6
endseg

beginseg
    name "object_zol"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_zol/object_zol.o"
    number 6
endseg

beginseg
    name "object_hammer_peg"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_hammer_peg/hammer_peg_base_model.o"
    include "$(BUILD_DIR)/assets/objects/object_hammer_peg/hammer_peg_top_model.o"
    include "$(BUILD_DIR)/assets/objects/object_hammer_peg/hammer_peg_base_col.o"
    number 6
endseg

beginseg
    name "object_ladder"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_medium_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_medium_col.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_small_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_small_col.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_large_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_large_col.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_medium_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_medium_col.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_small_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_small_col.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_large_model.o"
    include "$(BUILD_DIR)/assets/objects/object_ladder/ladder_metal_large_col.o"
    number 6
endseg

beginseg
    name "object_wiz"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_wiz/object_wiz.o"
    number 6
endseg

beginseg
    name "object_floor_spikes"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_small_col.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_small_model.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_spikes_small_model.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_medium_col.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_medium_model.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_spikes_medium_model.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_large_col.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_base_large_model.o"
    include "$(BUILD_DIR)/assets/objects/object_floor_spikes/floor_spike_spikes_large_model.o"
    number 6
endseg

beginseg
    name "object_pols_voice"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_skel.o"
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_anim_idle.o"
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_anim_hop.o"
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_anim_grab.o"
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_anim_damaged.o"
    include "$(BUILD_DIR)/assets/objects/object_pols_voice/pols_voice_anim_die.o"
    number 6
endseg

beginseg
    name "object_jumptoggle_platform"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_jumptoggle_platform/object_jumptoggle_platform.o"
    number 6
endseg

beginseg
    name "object_platforms"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_cone_grass_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_cone_grass_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_hex_grass_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_hex_grass_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_hex_ice_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_hex_ice_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_grass_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_grass_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_stone_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_stone_checkermodel.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_stone_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_wood_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_wood_checkermodel.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_wood_col.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_ice_model.o"
    include "$(BUILD_DIR)/assets/objects/object_platforms/platform_square_ice_col.o"
    number 6
endseg

beginseg
    name "object_wall_pull_switch"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_wall_pull_switch/object_wall_pull_switch.o"
    number 6
endseg

beginseg
    name "object_fan"
    compress
    romalign 0x1000
    include "$(BUILD_DIR)/assets/objects/object_fan/object_fan.o"
    include "$(BUILD_DIR)/assets/objects/object_fan/gWindDL.o"
    number 6
endseg
