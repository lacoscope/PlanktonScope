#include <wx/wx.h>
#include <wx/colour.h>
#include <wx/bitmap.h>
#include <filesystem>
#include <string>

#include "panels.h"
#include "main.h"
#include "colors.h"

#pragma once


class Fenetre : public wxFrame
{
public:
    Fenetre(const wxString& title);

	void OnQuit(wxCommandEvent & event);
	virtual bool ShouldInheritColours()const;
	void on_idle(wxIdleEvent & evt);
	
	IdPanel *id_p;
	MvtPanel *mvt_p;
	DispPanel *id_disp_p;
	DispPanel *mvt_disp_p;
	wxBitmap *img;
	
	wxPanel *parent_p;

	DECLARE_EVENT_TABLE()
};
