/**
 * Sfx Voice Bank
 *
 * DEFINE_SFX should be used for all sfx define in the voice bank from sequence 0
 *    - Argument 0: Channel name for sequence 0
 *    - Argument 1: Enum value for this sfx
 *    - Argument 2: Importance for deciding which sfx to prioritize. Higher values have greater importance
 *    - Argument 3: Slows the decay of volume with distance (a 2-bit number ranging from 0-3)
 *    - Argument 4: Applies increasingly random offsets to frequency (a 2-bit number ranging from 0-3)
 *    - Argument 5: Various flags to add properties to the sfx
 *
 * WARNING: entries must align with the table defined for the voice bank in sequence 0
 */
/* 0x6800 */ DEFINE_SFX(CHAN_615D, NA_SE_VO_LI_SWORD_N, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6801 */ DEFINE_SFX(CHAN_6175, NA_SE_VO_LI_SWORD_L, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6802 */ DEFINE_SFX(CHAN_619B, NA_SE_VO_LI_LASH, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6803 */ DEFINE_SFX(CHAN_61C1, NA_SE_VO_LI_HANG, 0x20, 2, 0, SFX_FLAG_10)
/* 0x6804 */ DEFINE_SFX(CHAN_61D5, NA_SE_VO_LI_CLIMB_END, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6805 */ DEFINE_SFX(CHAN_61EA, NA_SE_VO_LI_DAMAGE_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6806 */ DEFINE_SFX(CHAN_6201, NA_SE_VO_LI_FREEZE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6807 */ DEFINE_SFX(CHAN_6218, NA_SE_VO_LI_FALL_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6808 */ DEFINE_SFX(CHAN_622D, NA_SE_VO_LI_FALL_L, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6809 */ DEFINE_SFX(CHAN_6242, NA_SE_VO_LI_BREATH_REST, 0x30, 2, 1, SFX_FLAG_10)
/* 0x680A */ DEFINE_SFX(CHAN_6257, NA_SE_VO_LI_BREATH_DRINK, 0x30, 2, 1, SFX_FLAG_10)
/* 0x680B */ DEFINE_SFX(CHAN_625F, NA_SE_VO_LI_DOWN, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680C */ DEFINE_SFX(CHAN_626F, NA_SE_VO_LI_TAKEN_AWAY, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680D */ DEFINE_SFX(CHAN_6284, NA_SE_VO_LI_HELD, 0x50, 2, 0, SFX_FLAG_10)
/* 0x680E */ DEFINE_SFX(CHAN_6287, NA_SE_VO_LI_SNEEZE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x680F */ DEFINE_SFX(CHAN_6299, NA_SE_VO_LI_SWEAT, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6810 */ DEFINE_SFX(CHAN_62AB, NA_SE_VO_LI_DRINK, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6811 */ DEFINE_SFX(CHAN_62B4, NA_SE_VO_LI_RELAX, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6812 */ DEFINE_SFX(CHAN_62C6, NA_SE_VO_LI_SWORD_PUTAWAY, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6813 */ DEFINE_SFX(CHAN_649B, NA_SE_VO_LI_GROAN, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6814 */ DEFINE_SFX(CHAN_62CE, NA_SE_VO_LI_AUTO_JUMP, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6815 */ DEFINE_SFX(CHAN_62EC, NA_SE_VO_LI_MAGIC_NALE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6816 */ DEFINE_SFX(CHAN_62F4, NA_SE_VO_LI_SURPRISE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6817 */ DEFINE_SFX(CHAN_62FE, NA_SE_VO_LI_MAGIC_FROL, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6818 */ DEFINE_SFX(CHAN_6306, NA_SE_VO_LI_PUSH, 0x30, 2, 2, SFX_FLAG_10)
/* 0x6819 */ DEFINE_SFX(CHAN_630E, NA_SE_VO_LI_HOOKSHOT_HANG, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681A */ DEFINE_SFX(CHAN_6316, NA_SE_VO_LI_LAND_DAMAGE_S, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681B */ DEFINE_SFX(CHAN_631E, NA_SE_VO_LI_NULL_0x1b, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681C */ DEFINE_SFX(CHAN_6326, NA_SE_VO_LI_MAGIC_ATTACK, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681D */ DEFINE_SFX(CHAN_622D, NA_SE_VO_BL_DOWN, 0x80, 2, 0, SFX_FLAG_10)
/* 0x681E */ DEFINE_SFX(CHAN_632E, NA_SE_VO_LI_DEMO_DAMAGE, 0x30, 2, 0, SFX_FLAG_10)
/* 0x681F */ DEFINE_SFX(CHAN_615D, NA_SE_VO_LI_ELECTRIC_SHOCK_LV, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6820 */ DEFINE_SFX(CHAN_6336, NA_SE_VO_LI_SWORD_N_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6821 */ DEFINE_SFX(CHAN_634F, NA_SE_VO_LI_ROLLING_CUT_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6822 */ DEFINE_SFX(CHAN_619B, NA_SE_VO_LI_LASH_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6823 */ DEFINE_SFX(CHAN_6376, NA_SE_VO_LI_HANG_KID, 0x20, 2, 0, SFX_FLAG_10)
/* 0x6824 */ DEFINE_SFX(CHAN_639A, NA_SE_VO_LI_CLIMB_END_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6825 */ DEFINE_SFX(CHAN_63AF, NA_SE_VO_LI_DAMAGE_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6826 */ DEFINE_SFX(CHAN_63C6, NA_SE_VO_LI_FREEZE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6827 */ DEFINE_SFX(CHAN_63DD, NA_SE_VO_LI_FALL_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6828 */ DEFINE_SFX(CHAN_63F2, NA_SE_VO_LI_FALL_L_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6829 */ DEFINE_SFX(CHAN_6407, NA_SE_VO_LI_BREATH_REST_KID, 0x30, 2, 1, SFX_FLAG_10)
/* 0x682A */ DEFINE_SFX(CHAN_641C, NA_SE_VO_LI_BREATH_DRINK_KID, 0x30, 2, 1, SFX_FLAG_10)
/* 0x682B */ DEFINE_SFX(CHAN_6424, NA_SE_VO_LI_DOWN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682C */ DEFINE_SFX(CHAN_6434, NA_SE_VO_LI_TAKEN_AWAY_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682D */ DEFINE_SFX(CHAN_6449, NA_SE_VO_LI_HELD_KID, 0x50, 2, 0, SFX_FLAG_10)
/* 0x682E */ DEFINE_SFX(CHAN_6451, NA_SE_VO_LI_SNEEZE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x682F */ DEFINE_SFX(CHAN_6467, NA_SE_VO_LI_SWEAT_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6830 */ DEFINE_SFX(CHAN_6479, NA_SE_VO_LI_DRINK_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6831 */ DEFINE_SFX(CHAN_6482, NA_SE_VO_LI_RELAX_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6832 */ DEFINE_SFX(CHAN_6493, NA_SE_VO_LI_SWORD_PUTAWAY_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6833 */ DEFINE_SFX(CHAN_649B, NA_SE_VO_LI_GROAN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6834 */ DEFINE_SFX(CHAN_64A5, NA_SE_VO_LI_AUTO_JUMP_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6835 */ DEFINE_SFX(CHAN_64C9, NA_SE_VO_LI_MAGIC_NALE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6836 */ DEFINE_SFX(CHAN_64D1, NA_SE_VO_LI_SURPRISE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6837 */ DEFINE_SFX(CHAN_64DB, NA_SE_VO_LI_MAGIC_FROL_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6838 */ DEFINE_SFX(CHAN_64E3, NA_SE_VO_LI_PUSH_KID, 0x30, 1, 2, SFX_FLAG_10)
/* 0x6839 */ DEFINE_SFX(CHAN_64EB, NA_SE_VO_LI_HOOKSHOT_HANG_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683A */ DEFINE_SFX(CHAN_64F3, NA_SE_VO_LI_LAND_DAMAGE_S_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683B */ DEFINE_SFX(CHAN_64FB, NA_SE_VO_LI_NULL_0x1b_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683C */ DEFINE_SFX(CHAN_6503, NA_SE_VO_LI_MAGIC_ATTACK_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683D */ DEFINE_SFX(CHAN_63F2, NA_SE_VO_BL_DOWN_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683E */ DEFINE_SFX(CHAN_650B, NA_SE_VO_LI_DEMO_DAMAGE_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x683F */ DEFINE_SFX(CHAN_615D, NA_SE_VO_LI_ELECTRIC_SHOCK_LV_KID, 0x30, 2, 0, SFX_FLAG_10)
/* 0x6840 */ DEFINE_SFX(CHAN_612C, NA_SE_VO_NAVY_ENEMY, 0x60, 0, 0, SFX_FLAG_5)
/* 0x6841 */ DEFINE_SFX(CHAN_612C, NA_SE_VO_NAVY_HELLO, 0x30, 0, 0, SFX_FLAG_5)
/* 0x6842 */ DEFINE_SFX(CHAN_612C, NA_SE_VO_NAVY_HEAR, 0x30, 0, 0, SFX_FLAG_5)
/* 0x6843 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_NAVY_CALL, 0x60, 0, 0, 0)
/* 0x6844 */ DEFINE_SFX(CHAN_6513, NA_SE_VO_NA_HELLO_3, 0x30, 0, 0, 0)
/* 0x6845 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x45, 0x30, 0, 0, 0)
/* 0x6846 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x46, 0x30, 0, 0, 0)
/* 0x6847 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x47, 0x30, 0, 0, 0)
/* 0x6848 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x48, 0x30, 0, 0, 0)
/* 0x6849 */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x49, 0x30, 0, 0, 0)
/* 0x684A */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4a, 0x30, 0, 0, 0)
/* 0x684B */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4b, 0x30, 0, 0, 0)
/* 0x684C */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4c, 0x30, 0, 0, 0)
/* 0x684D */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4d, 0x30, 0, 0, 0)
/* 0x684E */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4e, 0x30, 0, 0, 0)
/* 0x684F */ DEFINE_SFX(CHAN_6581, NA_SE_VO_DUMMY_0x4f, 0x30, 0, 0, 0)
/* 0x6850 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_TA_SLEEP, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6851 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_TA_SURPRISE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6852 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_TA_CRY_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6853 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_TA_CRY_1, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6854 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_IN_CRY_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6855 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_IN_LOST, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6856 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_IN_LASH_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6857 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_IN_LASH_1, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6858 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_FR_LAUGH_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6859 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_FR_SMILE_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685A */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NB_AGONY, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685B */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NB_CRY_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685C */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NB_NOTICE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685D */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NA_HELLO_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685E */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NA_HELLO_1, 0x30, 1, 1, SFX_FLAG_15)
/* 0x685F */ DEFINE_SFX(CHAN_6520, NA_SE_VO_NA_HELLO_2, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6860 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_CRASH, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6861 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_DISCOVER, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6862 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_FALL, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6863 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_LAUGH_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6864 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_LIFT, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6865 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_THROW, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6866 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_RT_UNBALLANCE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6867 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_ST_DAMAGE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6868 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_ST_ATTACK, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6869 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_HURRY, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686A */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_MEET, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686B */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_QUESTION, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686C */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_SIGH_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686D */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_SMILE_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686E */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_SURPRISE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x686F */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z0_THROW, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6870 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_SK_CRY_0, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6871 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_SK_CRY_1, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6872 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_SK_CRASH, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6873 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_SK_LAUGH, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6874 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_SK_SHOUT, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6875 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z1_CRY_0, 0x30, 3, 1, SFX_FLAG_15)
/* 0x6876 */ DEFINE_SFX(CHAN_6520, NA_SE_VO_Z1_CRY_1, 0x30, 3, 1, SFX_FLAG_15)
/* 0x6877 */ DEFINE_SFX(CHAN_6562, NA_SE_VO_Z1_OPENDOOR, 0x30, 3, 1, SFX_FLAG_15)
/* 0x6878 */ DEFINE_SFX(CHAN_6562, NA_SE_VO_Z1_SURPRISE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6879 */ DEFINE_SFX(CHAN_6562, NA_SE_VO_Z1_PAIN, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687A */ DEFINE_SFX(CHAN_6562, NA_SE_VO_KZ_MOVE, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687B */ DEFINE_SFX(CHAN_6562, NA_SE_VO_NB_LAUGH, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687C */ DEFINE_SFX(CHAN_6562, NA_SE_VO_IN_LAUGH, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687D */ DEFINE_SFX(CHAN_6520, NA_SE_VO_DUMMY_0x7d, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687E */ DEFINE_SFX(CHAN_6520, NA_SE_VO_DUMMY_0x7e, 0x30, 1, 1, SFX_FLAG_15)
/* 0x687F */ DEFINE_SFX(CHAN_6520, NA_SE_VO_DUMMY_0x7f, 0x30, 1, 1, SFX_FLAG_15)
/* 0x6880 */ DEFINE_SFX(CHANNEL_WOOOO, NA_SE_VO_WOOOO, 0x30, 1, 0, SFX_FLAG_15)
