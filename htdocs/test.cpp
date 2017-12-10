#include<iostream>
#include<string>
using namespace std;

class HTML
{
public:
    void generate_form()
    {
        cout << "<h1>Test Form</h1>" << endl;
        cout << "<form action='test.cpp' method='GET'>" << endl;
        cout << "<input type='text' name='test_1'/><br>" << endl;
        cout << "<input type='text' name='test_2'/><br>" << endl;
        cout << "<input type='text' name='test_3'/><br>" << endl;
        cout << "<input type='submit'/><br>" << endl;
        cout << "</form>";
    }
};

int main(int argc, char** argv)
{
    // Essential HTTP Header
    cout << "HTTP/1.1 200 OK" << endl;
    cout << "Content-Type: text/html" << endl << endl;

    HTML obj;

    cout << "<html>" << endl;
    cout << "<head>" << endl;
    cout << "<title>Hello World in CPP</title>" << endl;
    cout << "</head>" << endl;
    cout << "<body>" << endl;

    obj.generate_form();

    cout << "<hr>" << endl;

    if(argc > 1){
        cout << argv[1] << endl;
    }

    cout << "</body>" << endl;
    cout << "</html>" << endl;

    return 0;
}
