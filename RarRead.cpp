#include <stdio.h> 
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <iterator>
#include <io.h>
#include <conio.h>
#include <math.h>

using namespace std;

typedef unsigned short ubyte;
typedef unsigned long int UBYTE;

int main(int argc, char** argv)
{

    FILE* fp;
    int width = 1628, height = 1628;
    int imageSize = width * height;
    ubyte* inimg;
    UBYTE* outimg;
    ubyte* meanimg;
    ubyte* minusimg;
    ubyte* mtfimg;
 
    int cnt = 0;

    vector<string> v;
    _finddata_t fd;
    long handle;
    int result = 1;
    handle = _findfirst("C:\\m_data_230110\\S1_1628x1628\\Dark\\*.raw", &fd);

    if (handle == -1)
    {
        return -1;
    }

    while (result != -1) {

        v.push_back(fd.name);

        result = _findnext(handle, &fd);
    }

    vector<string>::iterator iter = v.begin();
    inimg = (ubyte*)malloc(sizeof(ubyte) * imageSize);
    outimg = (UBYTE*)malloc(sizeof(UBYTE) * imageSize * 101);
    meanimg = (ubyte*)malloc(sizeof(ubyte) * imageSize );
    minusimg = (ubyte*)malloc(sizeof(ubyte) * imageSize);
    mtfimg = (ubyte*)malloc(sizeof(ubyte) * imageSize);
    for (iter = v.begin(); iter != v.end(); iter++) {
        string g = "C:\\m_data_230110\\S1_1628x1628\\Dark\\";
        string path = g + (*iter);
       
        

        if ((fp = fopen(path.c_str(), "rb")) == NULL) {
            fprintf(stderr, "Error : Failed to open file...₩n");
            return -1;
        }

        fread(inimg, sizeof(ubyte) * imageSize, 1 , fp);

        fclose(fp);

        if ((fp = fopen( "C:\\m_data_230110\\S1_1628x1628\\MTF_V.raw", "rb")) == NULL) {
            fprintf(stderr, "Error : Failed to open file...₩n");
            return -1;
        }

        fread(mtfimg, sizeof(ubyte) * imageSize, 1, fp);

        fclose(fp);


        for (int i = 0; i < imageSize; i++)
        {
            outimg[i + (imageSize * cnt)] = inimg[i]; 
        }
        cnt++;
    }

    int cnt2 = 0;
    
    float line = 0.0;
    
    for (int y = 0; y < imageSize; y++) {
        float sum = 0.0;
        for (int x=0; x < 101; x++)
        {
            sum += outimg[y + x * imageSize];
        }
        meanimg[y] = sum / 101;
        

    }

    for (int y = 0; y < imageSize; y++) {
        minusimg[y] = abs(mtfimg[y] - meanimg[y]);
    }



    if ((fp = fopen("C:\\m_data_230110\\S1_1628x1628\\minus.raw", "wb")) == NULL) {
        fprintf(stderr, "Error : can't open file...₩n");
        return -1;
    }
   
    fwrite(minusimg, sizeof(ubyte) * imageSize, 1, fp);

    fclose(fp);
    free(outimg);
    free(inimg);
    return 0;
}
