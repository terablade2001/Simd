#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <Simd\SimdDetection.hpp>
#include <Test\TestUtils.h>

#define __FNAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifndef __IRIDALABS__CLOCK_COUNTER__
#define __IRIDALABS__CLOCK_COUNTER__
#include <ctime>
#define INIT_TICTOC(x) static timeval start_##x; static timeval end_##x; static float DiffTime_##x;
#define TIC(x) (gettimeofday(&start_##x, NULL))
#define TOC(x) (gettimeofday(&end_##x, NULL)); \
	(DiffTime_##x = (((end_##x.tv_sec + end_##x.tv_usec * 1e-6) - (start_##x.tv_sec + start_##x.tv_usec * 1e-6)) * 1000))
#define TICTOC(x) (DiffTime_##x)
#endif

using namespace std;

typedef Simd::Detection<Simd::Allocator> Detection;

int main( int argc, char** argv ) {	
	INIT_TICTOC(T);
	
	
	Detection::View image(
		1920,                   /// Image Width
		1080,                   /// Image Height
		Detection::View::Gray8, /// Image Format
		NULL,                   /// ???
		64                      /// Alignment for NEON (???)
	);
	
	// Load the Binary uint8_t image data (1 Channel) to image, indirectly by a file.
	// i.e. convert img 11 faces: http://1img.org/wp-content/uploads/2016/06/Portugal-national-football-team-2.jpg
	// i.e. convert img 1 face: http://www.hdwallwide.com/wp-content/uploads/2014/06/hd-wallpaper-matt-damon-person-brunette-cardigan-1080p.jpg
	FILE* fp = fopen("FHD.bdat", "rb");
	if (fp == NULL) { printf("[%s: %i]: Error: FHD.bdat is not found!\n",__FNAME__,__LINE__); return -1; }
		fread(image.data,sizeof(uint8_t),1920*1080, fp);
	fclose(fp);
	
	// Initialize Detector
    Detection detection;
    detection.Load("lbp_face.xml");	
    detection.Init(image.Size());
    
	
	double TotalTime = 0.0;
	for (int i = 0; i < 20; i++) {
		Detection::Objects objects;
		TIC(T);
			detection.Detect(image, objects);
		TOC(T); TotalTime += TICTOC(T);
	}
	
	Detection::Objects objects;
	detection.Detect(image, objects);
	printf("[%s: %i]: Number of faces found: %lu\n",__FNAME__,__LINE__, objects.size());
    for (size_t i = 0; i < objects.size(); ++i) {
		printf("[%s: %i]: Face at: left: %li, top: %li, right: %li, bottom: %li\n",__FNAME__,__LINE__,
			objects[i].rect.left,
			objects[i].rect.top,
			objects[i].rect.right,
			objects[i].rect.bottom
		);
    }
	printf("[%s: %i]: Detection Time: %2.3f msec\n",__FNAME__,__LINE__, TotalTime / 20.0);
	return 0;
}

