#include "app.h"

int main()
{
    if (Initialize() == 0)
    {
        MainApplication *mainapp = new MainApplication();
        mainapp->Show();//WithFadeIn();
        delete mainapp;
        Finalize();
    }
    return 0;
}
