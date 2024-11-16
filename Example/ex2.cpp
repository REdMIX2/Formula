// #include <iostream>
// #include <fstream>
// #include <dlfcn.h>
// #include <stdlib.h>

// #include "structs.h"

// using namespace std;

// int main()
// {

//     // create own program
//     ofstream f("tmp.cpp");
//     f << "#include<stdlib.h>\n#include \"structs.h\"\n extern \"C\" void F(S &s) { s.a += s.a; s.b *= s.b; }\n";
//     f.close();

//     // create library
//     int i = system("/usr/bin/gcc -shared tmp.cpp -o libtmp.so");
//     if (i)
//     {
//     }
//     // load library
//     void *fLib = dlopen("./libtmp.so", RTLD_LAZY);
//     if (!fLib)
//     {
//         cerr << "Cannot open library: " << dlerror() << '\n';
//     }

//     if (fLib)
//     {
//         int (*fn)(S &) = dlsym(fLib, "F");

//         if (fn)
//         {
//             for (int i = 0; i < 11; i++)
//             {
//                 S s;
//                 s.a = i;
//                 s.b = i;

//                 // use function
//                 fn(s);
//                 cout << s.a << " " << s.b << endl;
//             }
//         }
//         dlclose(fLib);
//     }

//     return 0;
// }