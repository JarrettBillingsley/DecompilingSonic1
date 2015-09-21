// Centralized location for variable-clearing routines, so that cleared variables can be easily added as variable
// purposes are discovered

void Clear_Objects()
{
	memset(v_objspace, 0, sizeof(Object) * MAX_OBJECTS);
}

void Clear_NGFX_Buffer()
{
	memset(v_ngfx_buffer, 0, NGFX_BUFFER_LENGTH);
}

void Clear_Palette(void* pal)
{
	memset(pal, 0, 0x80);
}

void Clear_Emerald_List()
{
	memset(v_emldlist, 0, sizeof(v_emldlist));
}

void Clear_PLC_Buffer()
{
	memset(v_plc_buffer, 0, sizeof(v_plc_buffer));
}

void Clear_A400_AC00()
{
	memset(v_lvllayout, 0, sizeof(v_lvllayout));
	memset(v_ngfx_buffer, 0, sizeof(v_ngfx_buffer));
}

void Clear_F628_F680()
{
	v_vbla_routine = 0;
	v_spritecount = 0;
	v_pcyc_num = 0;
	v_pcyc_time = 0;
	v_random = 0;
	f_pause = false;
	v_vdp_buffer2 = 0;
	f_hbla_pal = false;
	v_waterpos1 = 0;
	v_waterpos2 = 0;
	v_waterpos3 = 0;
	f_water = false;
	v_wtr_routine = 0;
	f_wtr_state = false;
	memset(v_pal_buffer, 0, sizeof(v_pal_buffer));
}

void Clear_F6E4_F680()
{
	// blerp
}

void Clear_F700_F800()
{
	v_screenposx = 0;
	v_screenposy = 0;
	v_limitleft1 = 0;
	v_limitright1 = 0;
	v_limittop1 = 0;
	v_limitbtm1 = 0;
	v_limitleft2 = 0;
	v_limitright2 = 0;
	v_limittop2 = 0;
	v_limitbtm2 = 0;
	v_limitleft3 = 0;
	v_scrshiftx = 0;
	v_lookshift = 0;
	v_dle_routine = 0;
	f_nobgscroll = false;
	v_bgscroll1 = 0;
	v_bgscroll2 = 0;
	v_bgscroll3 = 0;
	f_bgscrollvert = false;
	v_sonspeedmax = 0;
	v_sonspeedacc = 0;
	v_sonspeeddec = 0;
	v_sonframenum = 0;
	f_sonframechg = false;
	v_anglebuffer = 0;
	v_opl_routine = 0;
	v_opl_screen = 0;
	memset(v_opl_data, 0, sizeof(v_opl_data));
	v_ssangle = 0;
	v_ssrotate = 0;
	v_btnpushtime1 = 0;
	v_btnpushtime2 = 0;
	v_palchgspeed = 0;
	v_collindex = 0;
	v_palss_num = 0;
	v_palss_time = 0;
	v_obj31ypos = 0;
	v_bossstatus = 0;
	v_trackpos = 0;
	f_lockscreen = false;
	v_256loop1 = 0;
	v_256loop2 = 0;
	v_256roll1 = 0;
	v_256roll2 = 0;
	v_lani0_frame = 0;
	v_lani0_time = 0;
	v_lani1_frame = 0;
	v_lani1_time = 0;
	v_lani2_frame = 0;
	v_lani2_time = 0;
	v_lani3_frame = 0;
	v_lani3_time = 0;
	v_lani4_frame = 0;
	v_lani4_time = 0;
	v_lani5_frame = 0;
	v_lani5_time = 0;
	v_gfxbigring = 0;
	f_conveyrev = false;
	memset(v_obj63, 0, sizeof(v_obj63));
	f_wtunnelmode = false;
	f_lockmulti = false;
	f_wtunnelallow = false;
	f_jumponly = false;
	v_obj6B = false;
	f_lockctrl = false;
	f_bigring = false;
	v_itembonus = 0;
	v_timebonus = 0;
	v_ringbonus = 0;
	f_endactbonus = false;
	v_sonicend = 0;
	memset(f_switch, 0, sizeof(f_switch));
}

void Clear_FE60_FF80()
{
	Clear_FE60_FF00();
}

void Clear_FE60_FF00()
{
	memset(v_oscillate + 1, 0, sizeof(ushort) * 32);
	v_ani0_time = 0;
	v_ani0_frame = 0;
	v_ani1_time = 0;
	v_ani1_frame = 0;
	v_ani2_time = 0;
	v_ani2_frame = 0;
	v_ani3_time = 0;
	v_ani3_frame = 0;
	v_ani3_buf = 0;
	v_limittopdb = 0;
	v_limitbtmdb = 0;
}