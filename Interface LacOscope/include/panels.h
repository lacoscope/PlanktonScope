#include <wx/wx.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include <Python.h>
#include <iostream>


#pragma once

#ifndef PANELSH
#define PANELSH

const int ID_PATH=100;
const int ID_RUN=101;
const int ID_RUN_MVT=102;
const int ID_RUN_IDENTIF=103;


class IdPanel : public wxPanel
{
public:
		IdPanel(wxPanel *parent);
		virtual ~IdPanel(){Py_Finalize();};
		
		void select_file(wxCommandEvent & event);
		virtual bool ShouldInheritColours()const;
		void run(wxCommandEvent & event);

		wxButton *path_select;
		wxButton *run_button;

		wxStaticText *path_text;
		wxStaticText *path_title;
		wxTextCtrl *console;

		wxString *path;
		
		PyObject *ut;
		PyObject *id;
		
		wxPanel *parent_panel;
};

class MvtPanel : public wxPanel
{
public:
		MvtPanel(wxPanel *parent);
		
		virtual ~MvtPanel(){Py_Finalize();};
		
		void select_path(wxCommandEvent & event);
		virtual bool ShouldInheritColours()const;
		void liste_fichiers(wxArrayString p);
		void run(wxCommandEvent & event);
				
		wxButton *path_select;
		wxButton *run_button;
		
		wxStaticText *path_text;
		wxStaticText *path_title;
		wxStaticText *files_list;
		wxTextCtrl *console;
		
		wxArrayString filespath;
		
		PyObject *ut;
		PyObject *mvt;
		


		wxPanel *parent_panel;
};


class DispPanel : public wxPanel
{
public:
		DispPanel(wxPanel *parent,wxString p);
		void paintEvent(wxPaintEvent & evt);
		void OnSize(wxSizeEvent& event);
		void update_img();
		void render(wxDC& dc);
		
		wxImage image;
		wxBitmap resized;
		int w, h;
		wxString path;
		
		wxPanel *parent_panel;
		
		DECLARE_EVENT_TABLE()
};



#endif
