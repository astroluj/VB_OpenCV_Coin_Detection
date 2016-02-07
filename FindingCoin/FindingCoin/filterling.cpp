#include "filterling.h"

// Dilate -> Erode -> Dilate
void Filterling ::noiseEraser (IplImage *srcImage, IplImage *dstImage) {
	int COL =3, ROW = 3, ITERATIONS = 1 ;

	IplConvKernel *elem =cvCreateStructuringElementEx (COL, ROW, 0, 0, CV_SHAPE_RECT, NULL) ;
	//cvMorphologyEx (img, img, NULL, elem, CV_MOP_CLOSE, 1) ; // ����
	cvDilate (srcImage, dstImage, elem, ITERATIONS) ;	// ��â
	cvErode (dstImage, dstImage, elem, ITERATIONS * 2) ;	//ħ��
	cvDilate (dstImage, dstImage, elem, ITERATIONS) ;	// ��â
}

// Histogram Contrast Stretching
void Filterling ::contrastStretching (IplImage *srcImage, IplImage *dstImage) {
	// histogram������ ���� ����� �迭�޸𸮸� �Ҵ� 
	 unsigned int *histogramArray = new unsigned int[256],
		*sumHistogramArray = new unsigned int[256] ;
	memset (histogramArray, 0, sizeof (unsigned int) * 256) ;
	memset (sumHistogramArray, 0, sizeof (unsigned int) * 256) ;

	// ������ histogram�� ��� 
	for (size_t i = 0 ; i < srcImage ->height ; i++) 
		for (size_t j = 0 ; j < srcImage ->width ; j++) 
			histogramArray[(unsigned int)cvGetReal2D (srcImage, i, j)]++ ;

	// histogram�� ����ȭ�� ���� ��� 
	unsigned int sum = 0 ;
	const float SCALE_FACTOR = 128.0f / (float)(srcImage ->height * srcImage ->width) ;

	for (size_t i = 0 ; i < 256 ; i++) {
		sum += histogramArray[i] ;
		sumHistogramArray[i] = (unsigned int)((sum * SCALE_FACTOR) + 0.5) ;
	}

	// LUT�ν� sumHistogramArray �迭�� ����Ͽ� ������ ��ȯ 
	for (size_t i = 0 ; i < srcImage ->height ; i++) 
		for (size_t j = 0 ; j < srcImage ->width ; j++) 
			// pixel indexing
			cvSetReal2D (dstImage, i, j, (double) sumHistogramArray[(unsigned int)cvGetReal2D (srcImage, i, j)]) ;

	delete [] (histogramArray) ;
	delete [] (sumHistogramArray) ;
}