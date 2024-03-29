/*-
 * Copyright (c) discoDSP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of copyright holders nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by discoDSP
 *        http://www.discodsp.com/ and contributors.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HighLife GUI Implementation                                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Highlife.h"
#include "HighLifeEditor.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sample editor/map editor area coords
int	const area_x = 117;
int const area_y = 196;
int	const area_w = 632;
int const area_h = 172;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_tweak_int(int& i_param,int const amount,int const min,int const max)
{
	int t_param = i_param + amount;

	if(t_param < min)
		t_param = min;

	if(t_param > max)
		t_param = max;

	i_param = t_param;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CHighLifeEditor::gui_verify_coord(int x,int y,int zx,int zy,int cx,int cy)
{
	return (x>=zx && x<=(zx+cx) && (y>=zy) && y<=(zy+cy));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_bitmap(Graphics* const hdc,int const x,int const y,int const w,int const h,int const xo,int const yo,Image* const phbitmap)
{
    hdc->drawImage (phbitmap, x, y, w, h, xo, yo, w, h, false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CHighLifeEditor::gui_bitmap_ab(Graphics* const hdc,int const x,int const y,int const w,int const h,int const xo,int const yo,Image* const phbitmap,int const trans)
{
    hdc->setOpacity (trans / 255.0);
	hdc->drawImage (phbitmap, x, y, w, h, xo, yo, w, h, false);
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_solid_rect(Graphics* const hdc,int const x,int const y,int const w,int const h,const Colour& color)
{
    hdc->setColour (color);
    hdc->fillRect (x, y, w, h);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_invert(Graphics* const hdc,int const x,int const y,int const w,int const h)
{
    hdc->setColour (Colours::white.withAlpha (0.35f));
    hdc->fillRect (x, y, w, h);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_set_int_mode(int const im)
{
	fx->voice[0].set_int_mode (im);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_mouse_down (int const xm_pos, int const ym_pos, const MouseEvent& e)
{
	// program pointer
	HIGHLIFE_PROGRAM* pprg=&fx->highlife_program[fx->user_program];

	// menu file
	if(gui_verify_coord(xm_pos,ym_pos,119,49,13,10))
	{
		menu_file.TrackPopUpMenu(this,119,59);

		return;
	}

	// menu view
	if(gui_verify_coord(xm_pos,ym_pos,166,49,13,10))
	{
		// check current view page
#ifdef _WIN32
		CheckMenuRadioItem(menu_view.GetMenuHandler(),0,1,user_gui_page,MF_BYPOSITION);
#endif
		menu_view.TrackPopUpMenu(this,166,59);

		return;
	}

	// menu options
	if(gui_verify_coord(xm_pos,ym_pos,213,49,13,10))
	{
#ifdef _WIN32
		// check current interpolation method
		CheckMenuRadioItem(menu_options.GetMenuHandler(),2,4,2+gui_get_int_mode(),MF_BYPOSITION);
#endif
		menu_options.TrackPopUpMenu(this,213,59);
		
		return;
	}

	// vsti program menu selection
	if(gui_verify_coord(xm_pos,ym_pos,120,66,156,22))
	{
#ifndef DISABLE_VST_HOST
		// create menu with vst programs
		if(fx->host_plug)
		{
			// reset menu
			menu_vst_program.Reset();

			// fill list
			for(int prg=0;prg<fx->host_plug->numPrograms;prg++)
			{
				char buf_prg[256];
				sprintf(buf_prg,"---");
				fx->host_plug->dispatcher(fx->host_plug,effGetProgramNameIndexed,prg,0,buf_prg,0.0f);

				char prg_str[128];
				sprintf(prg_str,"%.3d %s",prg+1,buf_prg);
				menu_vst_program.AddTextOption(prg_str);

#ifdef _WIN32
				ULONG mbb=0;

				if(prg>0 && (prg&0x1F)==0)
					mbb=MF_MENUBARBREAK;

				ModifyMenu(menu_vst_program.GetMenuHandler(),prg,MF_BYPOSITION+mbb,16384+1280+prg,prg_str);
#endif
			}

			// check current vst program selected
//			int const curr_vst_prg=fx->host_plug->dispatcher(fx->host_plug,effGetProgram,0,0,NULL,0.0f); // unused
#ifdef _WIN32
			CheckMenuRadioItem(menu_vst_program.GetMenuHandler(),0,fx->host_plug->numPrograms-1,curr_vst_prg,MF_BYPOSITION);
#endif
			menu_vst_program.TrackPopUpMenu(this,119,86);

    		return;
		}
#else
        AlertWindow::showMessageBox (AlertWindow::WarningIcon,
                                     T("Warning"),
                                     T("Sampling an external vst plugin is actually disabled due to a code rewrite !"));
        return;        
#endif
	}

	// highlife program menu selection
	if(gui_verify_coord(xm_pos,ym_pos,120,89,156,10))
	{
		menu_hl_program.Reset();

		// update menu with programs list
		for(int prg=0;prg<NUM_PROGRAMS;prg++)
		{
			char prg_str[128];
			sprintf(prg_str,"%.3d %s",prg+1,fx->highlife_program[prg].name);
			menu_hl_program.AddTextOption(prg_str);

#ifdef _WIN32
			ULONG mbb=0;

			if(prg>0 && (prg&0x1F)==0)
				mbb=MF_MENUBARBREAK;

			ModifyMenu(menu_hl_program.GetMenuHandler(),prg,MF_BYPOSITION+mbb,16384+1024+prg,prg_str);
#endif
		}
	
#ifdef _WIN32
		// check current vst program selected
		CheckMenuRadioItem(menu_hl_program.GetMenuHandler(),0,NUM_PROGRAMS-1,fx->user_program,MF_BYPOSITION);
#endif
		menu_hl_program.TrackPopUpMenu(this,119,100);

		return;
	}

	// highlife zone browse dropdown (performance / general)
	if(gui_verify_coord(xm_pos,ym_pos,120,103,156,10))
	{
		// reset browse menu
		menu_zone.Reset();

		// temp buffer
		char temp_buf[MAX_PATH];

		// fill all zone names
		for(int z=0;z<pprg->num_zones;z++)
		{
			sprintf(temp_buf,"%.3d %s",z+1,pprg->pzones[z].name);
			menu_zone.AddTextOption(temp_buf);
		}

		// fill zone browsing options
        if (pprg->num_zones)
        {
		    menu_zone.AddSeparator();
		    menu_zone.AddTextOption("Browse Sample...");
		    menu_zone.AddSeparator();
		}
		menu_zone.AddTextOption("Add New Zone...");			
        if (pprg->num_zones)
        {
		    menu_zone.AddTextOption("Delete Zone...");			
		    menu_zone.AddTextOption("Delete All Zones...");
		}
		
#ifdef _WIN32
		// check current selected zone and track menu
		CheckMenuRadioItem(menu_zone.GetMenuHandler(),0,pprg->num_zones-1,fx->user_sed_zone,MF_BYPOSITION);
#endif
		menu_zone.TrackPopUpMenu(this,119,114);

		return;
	}

	// main buttons (import, reset, panic)
	if(gui_verify_coord(xm_pos,ym_pos,297,67,13,10))fx->user_pressed=NUM_PARAMETERS+0;						// load plugin
	if(gui_verify_coord(xm_pos,ym_pos,334,67,13,10))fx->user_pressed=NUM_PARAMETERS+1;						// load wav
	if(gui_verify_coord(xm_pos,ym_pos,297,96,13,10))fx->user_pressed=NUM_PARAMETERS+2;						// plugin gui
	if(gui_verify_coord(xm_pos,ym_pos,334,96,13,10))fx->user_route_midi=!fx->user_route_midi;				// plugin midi
	if(gui_verify_coord(xm_pos,ym_pos,297,128,13,10))fx->user_pressed=NUM_PARAMETERS+3;						// plugin freeze
	if(gui_verify_coord(xm_pos,ym_pos,334,128,13,10))fx->user_pressed=NUM_PARAMETERS+4;						// unload plugin

	// freeze settings
	if(gui_verify_coord(xm_pos,ym_pos,145,122,19,10))fx->user_pressed=NUM_PARAMETERS+5;						// program number splits
	if(gui_verify_coord(xm_pos,ym_pos,201,122,19,10))menu_vel_splits.TrackPopUpMenu(this,200,133);			// vel split menu
	if(gui_verify_coord(xm_pos,ym_pos,258,122,19,10))menu_spl_length.TrackPopUpMenu(this,257,133);			// split length menu
	if(gui_verify_coord(xm_pos,ym_pos,145,137,19,10))fx->user_pressed=NUM_PARAMETERS+6;						// low note tweak
	if(gui_verify_coord(xm_pos,ym_pos,201,137,19,10))fx->user_pressed=NUM_PARAMETERS+7;						// high note tweak
	if(gui_verify_coord(xm_pos,ym_pos,258,137,19,10))fx->user_pressed=NUM_PARAMETERS+8;						// kbd step menu

	// master volume
	if(gui_verify_coord(xm_pos,ym_pos,40,56,24,24))fx->user_pressed=1;
	
	// wheel / pitchbend
	if(gui_verify_coord(xm_pos,ym_pos,36,312,12,50))fx->user_pressed=(NUM_PARAMETERS+22);
	if(gui_verify_coord(xm_pos,ym_pos,56,312,12,50))fx->user_pressed=(NUM_PARAMETERS+23);
	
	// program playmode switch
	if(gui_verify_coord(xm_pos,ym_pos,62,260,8,8))pprg->ply_mode=2;											// polyphonic
	if(gui_verify_coord(xm_pos,ym_pos,62,270,8,8))pprg->ply_mode=1;											// legato
	if(gui_verify_coord(xm_pos,ym_pos,62,280,8,8))pprg->ply_mode=0;											// mono

	// performance view program params
	if(fx->user_gui_page==0)
	{
		// program fx switches
		if(gui_verify_coord(xm_pos,ym_pos,143,289,13,10))pprg->efx_dft=!pprg->efx_dft;						// efx daft mode
		if(gui_verify_coord(xm_pos,ym_pos,129,316,13,10))pprg->efx_rck=!pprg->efx_rck;						// efx rock da discoteque mode
		if(gui_verify_coord(xm_pos,ym_pos,299,316,13,10))pprg->efx_del_cro=!pprg->efx_del_cro;				// efx delay cross
		if(gui_verify_coord(xm_pos,ym_pos,339,316,13,10))pprg->efx_del_syn=!pprg->efx_del_syn;				// efx delay sync
		if(gui_verify_coord(xm_pos,ym_pos,169,290,8,8))pprg->efx_chr=!pprg->efx_chr;						// efx chr on
		if(gui_verify_coord(xm_pos,ym_pos,169,317,8,8))pprg->efx_del=!pprg->efx_del;						// efx del on
		if(gui_verify_coord(xm_pos,ym_pos,169,344,8,8))pprg->efx_rev=!pprg->efx_rev;						// efx rev on

		// program fx parameters
		if(gui_verify_coord(xm_pos,ym_pos,220,288,12,12))fx->user_pressed=16;
		if(gui_verify_coord(xm_pos,ym_pos,260,288,12,12))fx->user_pressed=17;
		if(gui_verify_coord(xm_pos,ym_pos,300,288,12,12))fx->user_pressed=18;
		if(gui_verify_coord(xm_pos,ym_pos,340,288,12,12))fx->user_pressed=19;
		if(gui_verify_coord(xm_pos,ym_pos,220,315,12,12))fx->user_pressed=20;
		if(gui_verify_coord(xm_pos,ym_pos,260,315,12,12))fx->user_pressed=21;
		if(gui_verify_coord(xm_pos,ym_pos,220,342,12,12))fx->user_pressed=22;
		if(gui_verify_coord(xm_pos,ym_pos,260,342,12,12))fx->user_pressed=23;
		if(gui_verify_coord(xm_pos,ym_pos,300,342,12,12))fx->user_pressed=24;
		if(gui_verify_coord(xm_pos,ym_pos,116,288,12,12))fx->user_pressed=25;
	}

	// zone user iteration
	if(fx->user_sed_zone>=0 && fx->user_sed_zone<pprg->num_zones)
	{
		// get zone pointer
		HIGHLIFE_ZONE* pz=&pprg->pzones[fx->user_sed_zone];

		// zone pitchbend range
		if(gui_verify_coord(xm_pos,ym_pos,40,125,24,24))
			fx->user_pressed=137;

		// zone glide auto
		if(gui_verify_coord(xm_pos,ym_pos,23,206,8,8))
		{
			if(get_int_param(pz->glide_auto,NUM_GLIDE_MODES)==0)
				pz->glide_auto=0.75f;
			else
				pz->glide_auto=0.25f;
		}

		// glide amount
		if(gui_verify_coord(xm_pos,ym_pos,40,198,24,24))
			fx->user_pressed=144;

		// zone performance parameters gui check
		if(fx->user_gui_page==0)
		{
			// filter type switch
			if(gui_verify_coord(xm_pos,ym_pos,317,216,8,8))pz->flt_type=get_float_param(0,NUM_FILTER_MODES);
			if(gui_verify_coord(xm_pos,ym_pos,301,195,8,8))pz->flt_type=get_float_param(1,NUM_FILTER_MODES);
			if(gui_verify_coord(xm_pos,ym_pos,301,205,8,8))pz->flt_type=get_float_param(2,NUM_FILTER_MODES);
			if(gui_verify_coord(xm_pos,ym_pos,329,195,8,8))pz->flt_type=get_float_param(3,NUM_FILTER_MODES);
			if(gui_verify_coord(xm_pos,ym_pos,329,205,8,8))pz->flt_type=get_float_param(4,NUM_FILTER_MODES);

			// vel mod and vel amp
			if(gui_verify_coord(xm_pos,ym_pos,723,86,12,12))fx->user_pressed=133;
			if(gui_verify_coord(xm_pos,ym_pos,723,122,12,12))fx->user_pressed=134;

			// filter tracking
			if(gui_verify_coord(xm_pos,ym_pos,241,185,12,12))fx->user_pressed=138;
			if(gui_verify_coord(xm_pos,ym_pos,241,221,12,12))fx->user_pressed=139;

			// filter cutoff / resonance
			if(gui_verify_coord(xm_pos,ym_pos,120,196,24,24))fx->user_pressed=140;
			if(gui_verify_coord(xm_pos,ym_pos,186,196,24,24))fx->user_pressed=142;

			// amp envelope
			if(gui_verify_coord(xm_pos,ym_pos,391,97,24,24))fx->user_pressed=146;
			if(gui_verify_coord(xm_pos,ym_pos,457,97,24,24))fx->user_pressed=148;
			if(gui_verify_coord(xm_pos,ym_pos,523,97,24,24))fx->user_pressed=150;
			if(gui_verify_coord(xm_pos,ym_pos,587,97,24,24))fx->user_pressed=152;
			if(gui_verify_coord(xm_pos,ym_pos,653,97,24,24))fx->user_pressed=154;

			// mod envelope
			if(gui_verify_coord(xm_pos,ym_pos,391,307,24,24))fx->user_pressed=156;
			if(gui_verify_coord(xm_pos,ym_pos,457,307,24,24))fx->user_pressed=158;
			if(gui_verify_coord(xm_pos,ym_pos,523,307,24,24))fx->user_pressed=160;
			if(gui_verify_coord(xm_pos,ym_pos,587,307,24,24))fx->user_pressed=162;
			if(gui_verify_coord(xm_pos,ym_pos,653,307,24,24))fx->user_pressed=164;
			if(gui_verify_coord(xm_pos,ym_pos,717,307,24,24))fx->user_pressed=166;	

			// mod lfo
			if(gui_verify_coord(xm_pos,ym_pos,440,204,8,8))pz->mod_lfo_syn=!pz->mod_lfo_syn;
			if(gui_verify_coord(xm_pos,ym_pos,391,196,24,24))fx->user_pressed=168;
			if(gui_verify_coord(xm_pos,ym_pos,457,196,24,24))fx->user_pressed=170;
			if(gui_verify_coord(xm_pos,ym_pos,523,196,24,24))fx->user_pressed=172;
			if(gui_verify_coord(xm_pos,ym_pos,587,196,24,24))fx->user_pressed=174;
			if(gui_verify_coord(xm_pos,ym_pos,653,196,24,24))fx->user_pressed=176;
			if(gui_verify_coord(xm_pos,ym_pos,717,196,24,24))fx->user_pressed=178;

			// fx sends
			if(gui_verify_coord(xm_pos,ym_pos,180,288,12,12))fx->user_pressed=180;
			if(gui_verify_coord(xm_pos,ym_pos,180,315,12,12))fx->user_pressed=182;
			if(gui_verify_coord(xm_pos,ym_pos,180,342,12,12))fx->user_pressed=184;

			// begin automation
			//if(fx->user_pressed>0 && fx->user_pressed<NUM_PARAMETERS)
			//	fx->beginParameterChangeGesture(fx->user_pressed);
		}

		// zone editor gui check
		if(fx->user_gui_page==1)
		{
			// zone loop type
			if(gui_verify_coord(xm_pos,ym_pos,574,91,19,10))
			{
#ifdef _WIN32
				CheckMenuRadioItem(menu_zone_loop.GetMenuHandler(),0,4,pz->loop_mode,MF_BYPOSITION);
#endif
				menu_zone_loop.TrackPopUpMenu(this,573,102);

        		return;
			}

			// zone sample trigger
			if(gui_verify_coord(xm_pos,ym_pos,506,137,19,10))
			{
#ifdef _WIN32
				CheckMenuRadioItem(menu_zone_trigger.GetMenuHandler(),0,1,pz->midi_trigger,MF_BYPOSITION);
#endif
				menu_zone_trigger.TrackPopUpMenu(this,505,148);

        		return;
        	}

			// zone parameter tweaks
			if(gui_verify_coord(xm_pos,ym_pos,416,107,19,10))fx->user_pressed=NUM_PARAMETERS+14;	// tweak zone gain
			if(gui_verify_coord(xm_pos,ym_pos,466,107,19,10))fx->user_pressed=NUM_PARAMETERS+15;	// tweak zone pan
			if(gui_verify_coord(xm_pos,ym_pos,416,122,19,10))fx->user_pressed=NUM_PARAMETERS+16;	// tweak zone coarse
			if(gui_verify_coord(xm_pos,ym_pos,466,122,19,10))fx->user_pressed=NUM_PARAMETERS+17;	// tweak zone fine
			if(gui_verify_coord(xm_pos,ym_pos,602,122,13,10))pz->mp_synchro=!pz->mp_synchro;	    // tweak zone sync status
			if(gui_verify_coord(xm_pos,ym_pos,574,122,19,10))fx->user_pressed=NUM_PARAMETERS+18;	// tweak zone ticks sync
			if(gui_verify_coord(xm_pos,ym_pos,506,122,19,10))fx->user_pressed=NUM_PARAMETERS+19;	// tweak zone keycents
			if(gui_verify_coord(xm_pos,ym_pos,416,137,19,10))fx->user_pressed=NUM_PARAMETERS+20;	// tweak zone group
			if(gui_verify_coord(xm_pos,ym_pos,466,137,19,10))fx->user_pressed=NUM_PARAMETERS+21;	// tweak zone offby

            // kraken added a couple of zones
			if(gui_verify_coord(xm_pos,ym_pos,652,108,19,10))fx->user_pressed=NUM_PARAMETERS+30;	// tweak zone note min
			if(gui_verify_coord(xm_pos,ym_pos,700,108,19,10))fx->user_pressed=NUM_PARAMETERS+31;	// tweak zone note max
			if(gui_verify_coord(xm_pos,ym_pos,652,123,19,10))fx->user_pressed=NUM_PARAMETERS+32;	// tweak zone velo min
			if(gui_verify_coord(xm_pos,ym_pos,700,123,19,10))fx->user_pressed=NUM_PARAMETERS+33;	// tweak zone velo max

			// so did haszari - allow tweaking of the base key for the sample
			if(gui_verify_coord(xm_pos,ym_pos,570,108,19,10))fx->user_pressed=NUM_PARAMETERS+34;	// tweak zone note min

            // zone name and file
			if(gui_verify_coord(xm_pos,ym_pos,417,76,113,10))
			{
			    AlertWindow alert ("discoDSP HighLife - Rename Zone", "Give the zone a new name", AlertWindow::QuestionIcon);
                alert.addTextEditor ("name", String (pz->name));
                alert.addButton ("Ok", 1);
                alert.addButton ("Cancel", 2);
                
                int ret = alert.runModalLoop();
                if (ret == 1)
                    strncpy (pz->name, (const char*) alert.getTextEditorContents ("name"), MAX_PATH);
            }
			
			// zone layer mapping editor
			if(fx->user_editor_mode==0)
			{	
			}

			// zone sample wave editor
			if(fx->user_editor_mode==1)
			{	
				// check offsets
				for(int ci=pz->num_cues-1;ci>=0;ci--)
				{
					// get offset point
					int const cof=gui_sed_sample_to_coord(pz->cue_pos[ci]);

					// check for item pressed
					if(gui_verify_coord(xm_pos,ym_pos,cof,area_y+area_h-12,12,12))
					{
						// user_offset
						fx->user_sed_cue=ci;
						fx->user_pressed=NUM_PARAMETERS+13;

						// update mouse coords
						fx->user_ox=xm_pos;
						fx->user_oy=ym_pos;

						// refresh editor window
                        gui_refresh ();

						return;
					}
				}

				// get selectin width in pixels
				int const sel_w=fx->user_sed_sel_len/fx->user_sed_spp;

				// get loop points
				int const ls=gui_sed_sample_to_coord(pz->loop_start);
				int const le=gui_sed_sample_to_coord(pz->loop_end);

				if(pz->loop_mode && gui_verify_coord(xm_pos,ym_pos,le-12,area_y+area_h-12,12,12))
				{
					// loop end
					fx->user_pressed=NUM_PARAMETERS+12;
				}
				else if(pz->loop_mode && gui_verify_coord(xm_pos,ym_pos,ls,area_y+area_h-12,12,12))
				{
					// loop start
					fx->user_pressed=NUM_PARAMETERS+11;
				}
				else if(gui_verify_coord(xm_pos,ym_pos,gui_sed_sample_to_coord(fx->user_sed_sel_sta),area_y,4,area_h) && sel_w>=8)
				{
					// left finetune
					fx->user_pressed=NUM_PARAMETERS+9;
				}
				else if(gui_verify_coord(xm_pos,ym_pos,gui_sed_sample_to_coord(fx->user_sed_sel_sta+fx->user_sed_sel_len)-4,area_y,2,area_h) && sel_w>=8)
				{
					// right finetune
					fx->user_pressed=NUM_PARAMETERS+10;
				}
				else if(gui_verify_coord(xm_pos,ym_pos,area_x,area_y,area_w,area_h))
				{
					int const smp_pos=gui_sed_coord_to_sample(xm_pos);

					// check for double click
					if(smp_pos==fx->user_sed_sel_sta)
					{
						if(pz->loop_mode)
						{
							if(smp_pos<pz->loop_start)
							{
								fx->user_sed_sel_sta=0;
								fx->user_sed_sel_len=pz->loop_start;
							}
							else if(smp_pos>=pz->loop_start && smp_pos<pz->loop_end)
							{
								fx->user_sed_sel_sta=pz->loop_start;
								fx->user_sed_sel_len=pz->loop_end-pz->loop_start;
							}
							else
							{
								fx->user_sed_sel_sta=pz->loop_end;
								fx->user_sed_sel_len=pz->num_samples-pz->loop_end;
							}
						}
						else
						{
							fx->user_sed_sel_sta=0;
							fx->user_sed_sel_len=pz->num_samples;
						}
					}
					else
					{
						// sample selection
						fx->user_sed_sel_sta=smp_pos;
						fx->user_sed_sel_len=fx->user_sed_spp;
						fx->user_pressed=NUM_PARAMETERS+10;
						gui_sed_fix_sel (pz->num_samples, e.mods.isCommandDown () ? 1 : 0);
					}
				}

				// sample editor toolbar zoom in
				if(gui_verify_coord(xm_pos,ym_pos,154,188,16,8))
				{
					int const npp = fx->user_sed_sel_len / area_w;

					if (npp > 0) {
    					fx->user_sed_offset = fx->user_sed_sel_sta;
						fx->user_sed_spp = (npp + 1);
			        }
					else {
						fx->user_sed_spp /= 3;
			        }
				}

				// sample editor toolbar zoom out
				if(gui_verify_coord(xm_pos,ym_pos,170,188,16,8))
				{
					fx->user_sed_offset -= area_w * fx->user_sed_spp;

					if (fx->user_sed_offset < 0)
						fx->user_sed_offset = 0;

					fx->user_sed_spp *= 3;
				}

				// sample editor toolbar zoom full out
				if(gui_verify_coord(xm_pos,ym_pos,186,188,32,8))
				{
					fx->user_sed_offset = 0;
					fx->user_sed_spp = (pz->num_samples / area_w) + 1;
				}

				// sample editor toolbar menu edit dropdown
				if(gui_verify_coord(xm_pos,ym_pos,218,188,56,8))
				{
					menu_sample_edit.TrackPopUpMenu(this,218,196);
            		return;
                }
        		
				// sample editor toolbar menu loop dropdown
				if(gui_verify_coord(xm_pos,ym_pos,273,188,56,8))
				{
					menu_sample_loop.TrackPopUpMenu(this,273,196);
            		return;
                }

				// sample editor toolbar menu ampl dropdown
				if(gui_verify_coord(xm_pos,ym_pos,328,188,56,8))
				{
					menu_sample_ampl.TrackPopUpMenu(this,328,196);
            		return;
                }
	
				// sample editor toolbar menu filt dropdown
				if(gui_verify_coord(xm_pos,ym_pos,383,188,56,8))
				{
					menu_sample_filt.TrackPopUpMenu(this,383,196);
            		return;
                }
	
				// sample editor toolbar menu spfx dropdown
				if(gui_verify_coord(xm_pos,ym_pos,438,188,56,8))
				{
					menu_sample_spfx.TrackPopUpMenu(this,438,196);
            		return;
                }
				
				// sample editor toolbar menu cuep dropdown
				if(gui_verify_coord(xm_pos,ym_pos,493,188,56,8))
				{
					menu_sample_cuep.TrackPopUpMenu(this,493,196);
            		return;
                }
			}
		}
	}
	else
	{
		// no zone selected
	}

	// update mouse coords
	fx->user_ox = xm_pos;
	fx->user_oy = ym_pos;
	fx->user_delta_y_accum = 0;

	// refresh editor window
    gui_refresh ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_mouse_move (int const xm_pos, int const ym_pos, const MouseEvent& e)
{	
	// program pointer
	HIGHLIFE_PROGRAM* pprog=&fx->highlife_program[fx->user_program];
	
	// cursor grab
	bool grab_found=false;
	
	// sample editor cursor set
	if(fx->user_gui_page==1 && fx->user_editor_mode==1
	   && fx->user_sed_zone>=0 && fx->user_sed_zone<pprog->num_zones)
	{
		// get zone pointer
		HIGHLIFE_ZONE* pz=&pprog->pzones[fx->user_sed_zone];

		// middle select
		if(gui_verify_coord(xm_pos,ym_pos,area_x,area_y,area_w,area_h))
		{
			// get sel pixel length
			int const sel_w=fx->user_sed_sel_len/fx->user_sed_spp;

			// set beam cursor for selection mark
			grab_found=true;
            setMouseCursor (hcursor_beam);

			// get sample selection coords
			if(fx->user_sed_sel_len>0)
			{
				// left finetune
				if(gui_verify_coord(xm_pos,ym_pos,gui_sed_sample_to_coord(fx->user_sed_sel_sta),area_y,4,area_h) && sel_w>=8)
				{
					grab_found=true;
                    setMouseCursor (hcursor_szwe);
				}

				// right finetune
				if(gui_verify_coord(xm_pos,ym_pos,gui_sed_sample_to_coord(fx->user_sed_sel_sta+fx->user_sed_sel_len)-4,area_y,2,area_h) && sel_w>=8)
				{
					grab_found=true;
                    setMouseCursor (hcursor_szwe);
				}
			}
		}

		// check loop positions
		if(pz->loop_mode)
		{
			int const ls=gui_sed_sample_to_coord(pz->loop_start);
			int const le=gui_sed_sample_to_coord(pz->loop_end);

			if(gui_verify_coord(xm_pos,ym_pos,ls,area_y+area_h-12,12,12))
			{
				grab_found=true;
                setMouseCursor (hcursor_szwe);
			}

			if(gui_verify_coord(xm_pos,ym_pos,le-12,area_y+area_h-12,12,12))
			{
				grab_found=true;
                setMouseCursor (hcursor_szwe);
			}
		}

		// check cue positions
		for(int ci=0;ci<pz->num_cues;ci++)
		{
			int const of=gui_sed_sample_to_coord(pz->cue_pos[ci]);

			if(gui_verify_coord(xm_pos,ym_pos,of,area_y+area_h-12,12,12))
			{
				grab_found=true;
                setMouseCursor (hcursor_szwe);
			}
		}
	}

	// not found any grab, set default cursor
	if(grab_found==false)
        setMouseCursor (hcursor_arro);

	// check if user item press status is set
	if(fx->user_pressed)
	{
		// get mouse delta
		int const i_hdelta=fx->user_ox-xm_pos;
		int i_vdelta=fx->user_oy-ym_pos;

		if(i_vdelta>=-1 && i_vdelta<=1)
			i_vdelta=(fx->user_oy-ym_pos)*((ym_pos&3)==0);
		
		// check user pressed item index
		if(fx->user_pressed<NUM_PARAMETERS)
		{
			// vst params
			float f_dif=float(fx->user_oy-ym_pos)*0.005f;

			// finetuning
			if (e.mods.isShiftDown ())
				f_dif*=0.01f;

			// ctrl next param
			bool can_ctrl=false;
			int parameter_to_tweak;

			// set flag to allow sense control for parameters with modulation sense
			if(fx->user_pressed>=140)
				can_ctrl=true;

			// get parameter to tweak
			if (e.mods.isCommandDown () && can_ctrl)
				parameter_to_tweak=fx->user_pressed+1;
			else
				parameter_to_tweak=fx->user_pressed;

			// add parameter
			float curr_val=fx->getParameter(parameter_to_tweak)+f_dif;

			// clamp param
			if(curr_val<0.0f)curr_val=0.0f;
			if(curr_val>1.0f)curr_val=1.0f;

			// update param
			fx->setParameterNotifyingHost(parameter_to_tweak,curr_val);
		}
		else
		{
			// on-move program and enviroment params (all views)
			if(fx->user_pressed==(NUM_PARAMETERS+5)) gui_tweak_int(fx->user_prg_splits,i_vdelta,1,128);					    // tweak freezer num program splits
			if(fx->user_pressed==(NUM_PARAMETERS+6)) gui_tweak_int(fx->user_low_keyboa,i_vdelta,0,fx->user_hig_keyboa);		// tweak freezer low keyboard range
			if(fx->user_pressed==(NUM_PARAMETERS+7)) gui_tweak_int(fx->user_hig_keyboa,i_vdelta,fx->user_low_keyboa,119);	// tweak freezer hig keyboard range
			if(fx->user_pressed==(NUM_PARAMETERS+8)) gui_tweak_int(fx->user_ste_keyboa,i_vdelta,1,12);					    // tweak freezer ste keyboard range

			// tweak pitchbend
			if(fx->user_pressed==(NUM_PARAMETERS+22))
			{
				gui_tweak_int(fx->midi_state.midi_bend,i_vdelta*128,-8192,8192);
				fx->plug_ramp_all_voices();
			}

			// tweak modwheel
			if(fx->user_pressed==(NUM_PARAMETERS+23))
			{
				gui_tweak_int(fx->midi_state.midi_cc[1],i_vdelta,0,127);
				fx->plug_ramp_all_voices();
			}

			// sampled editor drags
			if(fx->user_sed_zone>=0 && fx->user_sed_zone<pprog->num_zones)
			{
				// get zone pointer
				HIGHLIFE_ZONE* pz=&pprog->pzones[fx->user_sed_zone];

				// wave drags operations
				if(fx->user_pressed>=(NUM_PARAMETERS+9) && fx->user_pressed<=(NUM_PARAMETERS+13))
				{
					// get new sample pos at cursor x
					int sed_pos=gui_sed_coord_to_sample(xm_pos);

					// clamp pos
					if(sed_pos<0)
						sed_pos=0;

					// selection left finetune
					if(fx->user_pressed==(NUM_PARAMETERS+9))
					{
						fx->user_sed_sel_len+=fx->user_sed_sel_sta-sed_pos;
						fx->user_sed_sel_sta=sed_pos;

						if(fx->user_sed_sel_sta<0)
							fx->user_sed_sel_sta=0;

						if(fx->user_sed_sel_len<fx->user_sed_spp)
							fx->user_sed_sel_len=fx->user_sed_spp;
					}

					// sample selection
					if(fx->user_pressed==(NUM_PARAMETERS+10))
					{
						int sel_len=(sed_pos+fx->user_sed_spp)-fx->user_sed_sel_sta;

						if(sel_len<=0)
						{
							fx->user_sed_sel_sta=sed_pos;
							sel_len=fx->user_sed_spp;
						}

						fx->user_sed_sel_len=sel_len;
					}

					// loop start set
					if(fx->user_pressed==(NUM_PARAMETERS+11))
					{
						gui_tweak_int(pz->loop_start,-i_hdelta*fx->user_sed_spp,0,pz->num_samples);
						int const nls=pz->loop_start;
						fx->setParameterNotifyingHost(129,float(nls)/float(pz->num_samples));
						pz->loop_start=nls;
					}

					// loop end set
					if(fx->user_pressed==(NUM_PARAMETERS+12))
					{
						gui_tweak_int(pz->loop_end,-i_hdelta*fx->user_sed_spp,0,pz->num_samples);
						int const nle=pz->loop_end;
						fx->setParameterNotifyingHost(130,float(nle)/float(pz->num_samples));
						pz->loop_end=nle;
					}

					// cue set
					if(fx->user_pressed==(NUM_PARAMETERS+13))
						gui_tweak_int(pz->cue_pos[fx->user_sed_cue],-i_hdelta*fx->user_sed_spp,0,pz->num_samples);

					// left scroller
					if(xm_pos<area_x)
						fx->user_sed_offset=sed_pos;

					// right scroller
					if(xm_pos>=(area_x+area_w))
						fx->user_sed_offset=sed_pos-area_w*fx->user_sed_spp;

					// fix selection
					gui_sed_fix_sel (pz->num_samples, e.mods.isCommandDown () ? 1 : 0);
				}

				// sample zone properties tweak
				if(fx->user_pressed==(NUM_PARAMETERS+14)) gui_tweak_int(pz->mp_gain,i_vdelta,-96,96);					// tweak zone gain
				if(fx->user_pressed==(NUM_PARAMETERS+15)) gui_tweak_int(pz->mp_pan,i_vdelta,-50,50);				    // tweak zone pan
				if(fx->user_pressed==(NUM_PARAMETERS+16)) gui_tweak_int(pz->midi_coarse_tune,i_vdelta,-36,36);			// tweak zone coarse tune
				if(fx->user_pressed==(NUM_PARAMETERS+17)) gui_tweak_int(pz->midi_fine_tune,i_vdelta,-50,50);			// tweak zone fine tune
				if(fx->user_pressed==(NUM_PARAMETERS+18)) gui_tweak_int(pz->mp_num_ticks,i_vdelta,1,999);				// tweak zone sync ticks
				if(fx->user_pressed==(NUM_PARAMETERS+19)) gui_tweak_int(pz->midi_keycents,i_vdelta,0,1024);				// tweak zone keycents
				if(fx->user_pressed==(NUM_PARAMETERS+20)) gui_tweak_int(pz->res_group,i_vdelta,0,999);					// tweak zone group
				if(fx->user_pressed==(NUM_PARAMETERS+21)) gui_tweak_int(pz->res_offby,i_vdelta,0,999);					// tweak zone offby

				if(fx->user_pressed==(NUM_PARAMETERS+30)) gui_tweak_int(pz->lo_input_range.midi_key,i_vdelta,0,pz->hi_input_range.midi_key);
				if(fx->user_pressed==(NUM_PARAMETERS+31)) gui_tweak_int(pz->hi_input_range.midi_key,i_vdelta,pz->lo_input_range.midi_key,119);
				if(fx->user_pressed==(NUM_PARAMETERS+32)) gui_tweak_int(pz->lo_input_range.midi_vel,i_vdelta,0,pz->hi_input_range.midi_vel);
				if(fx->user_pressed==(NUM_PARAMETERS+33)) gui_tweak_int(pz->hi_input_range.midi_vel,i_vdelta,pz->lo_input_range.midi_vel,127);

				if(fx->user_pressed==(NUM_PARAMETERS+34)) gui_tweak_int(pz->midi_root_key, i_vdelta, 0, 119);
			}
		}

		// refresh window
        gui_refresh ();
	}

	// update mouse coords
	fx->user_ox=xm_pos;
	fx->user_oy=ym_pos;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_sed_center_around_sel(void)
{
	int nd=fx->user_sed_sel_sta-fx->user_sed_spp*(area_w/2);

	if(nd<0)
		nd=0;

	fx->user_sed_offset=nd;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CHighLifeEditor::gui_sed_sample_to_coord(int const sample)
{
	return area_x+(sample-fx->user_sed_offset)/fx->user_sed_spp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CHighLifeEditor::gui_sed_coord_to_sample(int const x)
{
	return fx->user_sed_offset+(x-area_x)*fx->user_sed_spp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_sed_fix_sel (int const wave_len, int const snap)
{
	// program pointer
	HIGHLIFE_PROGRAM* pprog=&fx->highlife_program[fx->user_program];

	// synchro snap
	if (snap/*GetKeyState(VK_CONTROL)<0*/
	    && fx->user_sed_zone>=0 && fx->user_sed_zone<pprog->num_zones)
	{
		// get zone pointer
		HIGHLIFE_ZONE* pz=&pprog->pzones[fx->user_sed_zone];

		// snap to sync
		if(pz->mp_synchro)
		{
			// get tick size
			double const d_tick_len=double(pz->num_samples)/double(pz->mp_num_ticks);
			int const i_tick_index=fx->user_sed_sel_sta/int(d_tick_len);

			// set selection
			fx->user_sed_sel_sta=int(double(i_tick_index)*double(d_tick_len));
			fx->user_sed_sel_len/=int(d_tick_len);
			fx->user_sed_sel_len*=int(d_tick_len);
		}
	}

	if(fx->user_sed_sel_sta<0)
		fx->user_sed_sel_sta=0;

	if(fx->user_sed_sel_sta>=wave_len)
		fx->user_sed_sel_sta=wave_len-1;

	if(fx->user_sed_sel_len<fx->user_sed_spp)
		fx->user_sed_sel_len=fx->user_sed_spp;

	if((fx->user_sed_sel_sta+fx->user_sed_sel_len)>wave_len)
		fx->user_sed_sel_len=wave_len-fx->user_sed_sel_sta;

	if(fx->user_sed_offset>wave_len)
		fx->user_sed_offset=wave_len;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_mouse_up (int const xm_pos, int const ym_pos, const MouseEvent& e)
{
	// main buttons
#ifndef DISABLE_VST_HOST
	if(fx->user_pressed==(NUM_PARAMETERS+0))fx->host_open_vst_dialog();
	if(fx->user_pressed==(NUM_PARAMETERS+2))fx->host_open_plugin_editor();
	if(fx->user_pressed==(NUM_PARAMETERS+3))fx->host_generate_preset();
	if(fx->user_pressed==(NUM_PARAMETERS+4))fx->host_instance_free();
#endif

	if(fx->user_pressed==(NUM_PARAMETERS+1))fx->tool_sample_import_dlg();
	if(fx->user_pressed==(NUM_PARAMETERS+22))fx->midi_state.midi_bend=0;

	// notify host end of automation
	//if(fx->user_pressed>0 && fx->user_pressed<NUM_PARAMETERS)
	//	fx->endParameterChangeGesture (fx->user_pressed);

	// set user press state to 0
	fx->user_pressed=0;
	
	// refresh window
    gui_refresh ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_refresh()
{
//    const MessageManagerLock mml;
    repaint ();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_timer()
{
	// program pointer
	HIGHLIFE_PROGRAM* pprog=&fx->highlife_program[fx->user_program];

	// draw sample tracks
	if(fx->user_sed_zone>=0 && fx->user_sed_zone<pprog->num_zones
	   && fx->user_gui_page==1 && fx->user_editor_mode==1)
	{
#ifdef _WIN32
		// region for sample tracks
		HRGN hrgn=CreateRectRgn(area_x,area_y,area_x+area_w,area_y+area_h);
		SelectClipRgn(hdc,hrgn);
		DeleteObject(hrgn);
		hrgn=NULL;
#endif

#if 0 // we cannot draw directly inside Timer
		// scan voice tracks
		for(int v=0;v<MAX_POLYPHONY;v++)
		{
			CHighLifeVoice* pv=&voice[v];

			if(pv->is_working() && pv->playzone==user_sed_zone)
			{
				int const npos=gui_sed_sample_to_coord(int(pv->d_wave_phase));

				if(npos!=vt_opos[v])
				{
					if(gui_recent_update)
						gui_recent_update=false;
					else
						gui_invert(hdc,vt_opos[v],area_y,1,area_h);
					
					gui_invert(hdc,npos,area_y,1,area_h);
					vt_opos[v]=npos;
				}
			}
			else
			{
				if(vt_opos[v]>=0)
				{
					gui_invert(hdc,vt_opos[v],area_y,1,area_h);
					vt_opos[v]=0;
				}
			}
		}
#endif

#ifdef _WIN32
		SelectClipRgn(hdc,hrgn);
#endif
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHighLifeEditor::gui_command(int const lw_wpar)
{	
	HIGHLIFE_PROGRAM* pprg=&fx->highlife_program[fx->user_program];

	// menu file check
	if(menu_file.VerityCommandID(lw_wpar)>=0)
	{
		int const mo_index=menu_file.VerityCommandID(lw_wpar);

		// reset program
		if (mo_index==0 &&
            AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - New Program", "Are You Sure?"))
		{
			fx->set_suspended (1);
			fx->tool_init_program(pprg);
			fx->set_suspended (0);
		}

		// reset bank
		if (mo_index==1 &&
            AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - New Bank", "Are You Sure?"))
		{
			fx->set_suspended (1);
			fx->tool_init_bank();
			fx->set_suspended (0);
		}

		// import sample
		if(mo_index==3)
			fx->tool_sample_import_dlg();
		
		// import akp
		if(mo_index==4)
			fx->tool_program_import_dlg();

		// export wav
		if(mo_index==6)
			fx->wav_export();

		// export sfz
		if(mo_index==7)
			fx->sfz_export();
	}

	// menu view check
	if(menu_view.VerityCommandID(lw_wpar)>=0)
		fx->user_gui_page=menu_view.VerityCommandID(lw_wpar);

	// menu options check
	if(menu_options.VerityCommandID(lw_wpar)>=0)
	{
		int const mo_index=menu_options.VerityCommandID(lw_wpar);

		// panic
		if(mo_index==0)
			fx->plug_all_sounds_off();

        // set change zone on midi
		if(mo_index==2)
		    fx->user_sed_manual = ! fx->user_sed_manual;

		// interpolation
		if(mo_index>=4 && mo_index<=6)
			gui_set_int_mode(mo_index-4);
	}

	// menu vsti program selector menu
#ifndef DISABLE_VST_HOST
	if(menu_vst_program.VerityCommandID(lw_wpar)>=0 && fx->host_plug)
		fx->host_plug->dispatcher(fx->host_plug,effSetProgram,0,menu_vst_program.VerityCommandID(lw_wpar),NULL,0.0f);
#endif

	// menu highlife program selector menu
	if(menu_hl_program.VerityCommandID(lw_wpar)>=0)
		fx->setCurrentProgram(menu_hl_program.VerityCommandID(lw_wpar));

	// menu vel splits check
	if(menu_vel_splits.VerityCommandID(lw_wpar)>=0)
	{
		int const mo_index=menu_vel_splits.VerityCommandID(lw_wpar);
		fx->user_vel_splits = int (pow (2, mo_index));
	}

	// menu split lenght check
	if(menu_spl_length.VerityCommandID(lw_wpar)>=0)
	{
		int const mo_index=menu_spl_length.VerityCommandID(lw_wpar);
		fx->user_split_time = float (pow (2, mo_index));
	}

	// check for add zone
    if (menu_zone.VerityCommandID (lw_wpar) >= 0 && pprg->num_zones == 0)
	{
		int const mo_index = menu_zone.VerityCommandID (lw_wpar);

        if (mo_index == 0 
            && AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - Add New Zone", "Are You Sure?"))
        {
	        // add zone
	        fx->set_suspended (1);
	        fx->tool_add_zone (pprg);
	        fx->set_suspended (0);
        }
	}

	// check for zone selection
	if(fx->user_sed_zone>=0 && fx->user_sed_zone<pprg->num_zones)
	{
		// get zone pointer
		HIGHLIFE_ZONE* pz=&pprg->pzones[fx->user_sed_zone];

		// zone selector browser
		if (menu_zone.VerityCommandID (lw_wpar) >= 0)
		{
			int const mo_index = menu_zone.VerityCommandID (lw_wpar);

		    if(mo_index<pprg->num_zones)
		    {
			    // select zone
			    fx->user_sed_zone=mo_index;
		    }
		    else if (mo_index == (pprg->num_zones + 1))
		    {
			    // browse sample
			    fx->tool_sample_browse_dlg(pz);
		    }
	        else if (mo_index == (pprg->num_zones + 3)
	            && AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - Add New Zone", "Are You Sure?"))
	        {
		        // add zone
		        fx->set_suspended (1);
		        fx->tool_add_zone (pprg);
		        fx->set_suspended (0);
	        }
		    else if (mo_index == (pprg->num_zones + 4) 
                    && AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - Delete Zone", "Are You Sure?"))
		    {
			    // remove zone
			    fx->set_suspended (1);
			    fx->tool_delete_zone(pprg,fx->user_sed_zone);

			    // decrement selected zone
			    if(fx->user_sed_zone>0 && fx->user_sed_zone>=pprg->num_zones)
				    fx->user_sed_zone=pprg->num_zones-1;
			
			    fx->set_suspended (0);
		    }
		    else if (mo_index == (pprg->num_zones + 5)
                    && AlertWindow::showOkCancelBox (AlertWindow::QuestionIcon, "discoDSP HighLife - Delete All Zones", "Are You Sure?"))
		    {
			    // remove all zones
			    fx->set_suspended (1);
			    fx->tool_delete_all_zones(pprg);
			    fx->set_suspended (0);
		    }

			// adapt view
			fx->user_sed_adapt = 1;
		}

		// zone loop menu
		if(menu_zone_loop.VerityCommandID(lw_wpar)>=0)
			pz->loop_mode=menu_zone_loop.VerityCommandID(lw_wpar);

		// zone trigger menu
		if(menu_zone_trigger.VerityCommandID(lw_wpar)>=0)
			pz->midi_trigger=menu_zone_trigger.VerityCommandID(lw_wpar);

		// sample editor edit menu
		if(menu_sample_edit.VerityCommandID(lw_wpar)>=0)
		{
			int const mo_index=menu_sample_edit.VerityCommandID(lw_wpar);

			if(mo_index==0)fx->sed_sel_cut();	// cut
			if(mo_index==1)fx->sed_sel_copy();	// copy
			if(mo_index==2)fx->sed_sel_paste();	// paste
			if(mo_index==3)fx->sed_sel_trim();	// trim
			
			// select all
			if(mo_index==5)
			{
				fx->user_sed_sel_sta=0;
				fx->user_sed_sel_len=pz->num_samples;
			}
		}

		// sample editor loop menu
		if(menu_sample_loop.VerityCommandID(lw_wpar)>=0)
		{
			int const mo_index=menu_sample_loop.VerityCommandID(lw_wpar);

			// set loop start from sel start
			if(mo_index==0)
			{
				pz->loop_start=fx->user_sed_sel_sta;
				pz->loop_mode=1;
			}

			// set loop start from sel start
			if(mo_index==1)
			{
				pz->loop_end=fx->user_sed_sel_sta+fx->user_sed_sel_len;
				pz->loop_mode=1;
			}

			// set loop from selection
			if(mo_index==2)
			{
				pz->loop_start=fx->user_sed_sel_sta;
				pz->loop_end=fx->user_sed_sel_sta+fx->user_sed_sel_len;
				pz->loop_mode=1;
			}
		}

		// sample editor ampl menu
		if(menu_sample_ampl.VerityCommandID(lw_wpar)>=0)
		{
			int const mo_index=menu_sample_ampl.VerityCommandID(lw_wpar);

			if(mo_index==0)fx->sed_sel_vol_change(0.0f,1.0f);		// fade in
			if(mo_index==1)fx->sed_sel_vol_change(1.0f,0.0f);		// fade out
			if(mo_index==3)fx->sed_sel_normalize();					// normalize
			if(mo_index==4)fx->sed_sel_dc_remove();					// dc remove
		}

		// sample editor filt menu
		if(menu_sample_filt.VerityCommandID(lw_wpar)>=0)
		{
			int const mo_index=menu_sample_filt.VerityCommandID(lw_wpar);

			if(mo_index==0)fx->sed_sel_1st_order_iir(0.75f);		// smooth
			if(mo_index==1)fx->sed_sel_1st_order_iir(1.25f);		// enhance
		}

		// sample editor spfx menu
		if (menu_sample_spfx.VerityCommandID(lw_wpar) >= 0)
		{
			int const mo_index = menu_sample_spfx.VerityCommandID (lw_wpar);

			if (mo_index == 0) fx->sed_sel_reverse ();					// reverse
			if (mo_index == 1) fx->sed_sel_rectify ();					// rectify
			if (mo_index == 3) fx->sed_sel_mathdrive (0);				// sin drive
			if (mo_index == 4) fx->sed_sel_mathdrive (1);				// tanh drive
			if (mo_index == 6) fx->sed_sel_mathdrive (2);				// spectral halve
			if (mo_index == 8) fx->sed_time_stretch (0.5, 1.0);         // pitch shift .5x
			if (mo_index == 9) fx->sed_time_stretch (2.0, 1.0);         // pitch shift 2.x
			if (mo_index == 11) fx->sed_time_stretch (0.0, 0.5);        // time stretch .5x
			if (mo_index == 12) fx->sed_time_stretch (0.0, 2.0);        // time stretch 2.x
		}

		// sample editor cuep menu
		if(menu_sample_cuep.VerityCommandID(lw_wpar)>=0)
		{
			int const mo_index=menu_sample_cuep.VerityCommandID(lw_wpar);

			if(mo_index==0)fx->sed_add_cue();						// add cue
			if(mo_index==1)fx->sed_sel_remove_cues();				// remove cues within sel
		}
	}
	
	
	// refresh
    gui_refresh ();
}

