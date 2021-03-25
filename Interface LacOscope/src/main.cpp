#include "main.h"
#include "frame.h"

IMPLEMENT_APP(MyApp)

using namespace std;
bool MyApp::OnInit()
{

	wxInitAllImageHandlers();
    Fenetre *f = new Fenetre(wxT("LacOscope"));
    f->Show(true);

    return true;
}

string get_path_linux()
{
	char result[ PATH_MAX ];
	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	string s=string( result, (count > 0) ? count : 0 );
	s=s.substr(0, (s.substr(0, s.find_last_of("\\/"))).find_last_of("\\/"));
	return s;
}

//TODO: redefinir pour windows/mac ?
string get_path_windows()
{
	string s=string("windows");
	return s;
}
