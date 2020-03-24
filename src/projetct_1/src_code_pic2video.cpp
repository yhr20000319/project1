#include"src_code_pic2video.h"
#include <windows.h>
void video2result_pic()
{
	system("ffmpeg -f image2 -r 10 -i .\\src_code_pic\\%d.jpg -vcodec mpeg4 -t 1 .\\video\\tmp.mp4");
	//system("pause");
}