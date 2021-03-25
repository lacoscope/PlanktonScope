#include "frame.h"
#include <stdlib.h>


using namespace std;

#define STYLE (wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxMAXIMIZE)
wxString MVT_IMG_PATH=wxString(getexepath().append("/img/mouvement.jpg"));
wxString ID_IMG_PATH=wxString(getexepath().append("/img/segmentation.jpg"));
wxString ICON_IMG_PATH=wxString(getexepath().append("/img/plankton.jpeg"));

BEGIN_EVENT_TABLE(Fenetre, wxFrame)

EVT_IDLE(Fenetre::on_idle)

END_EVENT_TABLE()

Fenetre::Fenetre(const wxString& title):wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(250, 150),STYLE){
		
	SetIcon(wxIcon(ICON_IMG_PATH));
	
	
	parent_p = new wxPanel(this, wxID_ANY);
	

	wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *ligne1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *ligne2 = new wxBoxSizer(wxHORIZONTAL);
	
	mvt_p=new MvtPanel(parent_p);
	id_p=new IdPanel(parent_p);
	
	mvt_disp_p=new DispPanel(parent_p,MVT_IMG_PATH);
	id_disp_p=new DispPanel(parent_p,ID_IMG_PATH);
	
	ligne1->Add(id_p,1,wxEXPAND | wxALL,5);
	ligne1->Add(id_disp_p,2,wxEXPAND | wxALL,5);

	ligne2->Add(mvt_p,1,wxEXPAND | wxALL,5);
	ligne2->Add(mvt_disp_p,2,wxEXPAND | wxALL,5);
	
	Py_Initialize();
	PyObject* pName = PyUnicode_FromString("init");
	PyObject* init = PyImport_Import(pName);
	PyObject* pFunc = PyObject_GetAttrString(init, "module_check");
	if(pFunc && PyCallable_Check(pFunc))
		{
			wxStreamToTextRedirector redirect(this->id_p->console);
			PyObject *pValue = PyObject_CallObject(pFunc,NULL);
			const char *s=PyUnicode_AsUTF8(pValue);
			cout<<s<<endl;
		}
	
	vbox->Add(ligne1,1,wxEXPAND | wxALL,5);
	vbox->Add(ligne2,1,wxEXPAND | wxALL,5);
	
	
	parent_p->SetSizer(vbox);
	
	this->Centre();
	
	mvt_p->SetBackgroundColour(DARK_GRAY);
	id_p->SetBackgroundColour(DARK_GRAY);
	mvt_disp_p->SetBackgroundColour(DARK_GRAY);
	id_disp_p->SetBackgroundColour(DARK_GRAY);
	parent_p->SetOwnBackgroundColour(LIGHT_GREEN);
	
}

void Fenetre::on_idle(wxIdleEvent & evt){
	evt.RequestMore();
}

void Fenetre::OnQuit(wxCommandEvent & WXUNUSED(event))
{
    Close(true);
}

bool Fenetre::ShouldInheritColours()const{
	return false;
}


