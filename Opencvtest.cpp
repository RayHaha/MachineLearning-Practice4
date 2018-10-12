#include "stdafx.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include<fstream>
#include<string.h>
#include <time.h>

using namespace cv;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//Q20
	char theline [100] ;
	
	//read file
	fstream fin;
	fin.open("train.txt",ios::in);
	double data[500][100];


	//initial
	for(int i=0; i<500; i++){
		for(int j=0; j<100; j++){
			data[i][j] = 0 ;
		}
	}
	//分割字串存入二維陣列
	char *token = NULL;
	char *next_token = NULL;
	char seps[]   = " ,\t\n";
	int a=0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b = 0;
		token = strtok_s( theline, seps, &next_token);
		while (token != NULL){
			data[a][b] = atof(token);
            token = strtok_s( NULL, seps, &next_token);
			b++;
		}
		a++;
	}
	fin.close();
	fin.open("test.txt",ios::in);
	double test[500][100];
	//initial
	for(int i=0; i<500; i++){
		for(int j=0; j<100; j++){
			test[i][j] = 0 ;
		}
	}
	//存test的資料
	char *token2 = NULL;
	char *next_token2 = NULL;
	a = 0;
	while(fin.getline(theline,sizeof(theline),'\n')){
		int b=0;
		token2 = strtok_s( theline, seps, &next_token2);
		while (token2 != NULL){
			test[a][b] = atof(token2);
            token2 = strtok_s( NULL, seps, &next_token2);
			b++;
		}
		a++;
	}
	
	int checkran[500] ;
	for(int i=0; i<500; i++){
		checkran[i] = 0;
	}

	
	int result = 0;

	int his[100];
	for(int i=0; i<100; i++){
		his[i] = 0;
	}
	double totalerr = 0;
	//run 2000 times
	for(int t=0; t<2000; t++){
		bool again = true;
		double h = 0;
		double w[5] = {0,0,0,0,0};
		int start = 0;
		int countstop = 0;
		int count = 0;
		int number = 0;
		int order[500] ;
		for(int i=0; i<500; i++){
			checkran[i] = 0;
			order[i] = 0;
		}
		double wpocket[5] = {0,0,0,0,0};
		double error = 0;
		int cerr = 0;
		srand(t); // 以時間序列當亂數種子
		while(count!=499){
			number = rand()%500;
			if(checkran[number] == 1){

			}else{
				checkran[number] = 1;
				order[count] = number;
				count++;
			}
		}
		count = 0;
		//start PLA
		bool take = true;
		while(again){

			//take pocket
			if(take){
				int pocerror = 0;
				int nowerror = 0;
				for(int i=0; i<500; i++){
					double poctemp = wpocket[0]*1 + wpocket[1]*data[i][0] +  wpocket[2]*data[i][1] + wpocket[3]*data[i][2] + wpocket[4]*data[i][3] ;
					double nowtemp = w[0]*1 + w[1]*data[i][0] +  w[2]*data[i][1] + w[3]*data[i][2] + w[4]*data[i][3] ;
					if(poctemp>0){
						h = 1 ;
					}else{
						h = -1;
					}
					if(h == data[i][4]){

					}else{
						pocerror++;
					}
					if(nowtemp>0){
						h = 1 ;
					}else{
						h = -1;
					}
					if(h == data[i][4]){

					}else{
						nowerror++;
					}
				}

				if(nowerror<pocerror){
					for(int i=0; i<5; i++){
						wpocket[i] = w[i] ;
					}
				}
			}

			again = false;
			countstop++;
			number = order[start];
			double temp = w[0]*1 + w[1]*data[number][0] +  w[2]*data[number][1] + w[3]*data[number][2] + w[4]*data[number][3] ;
			if(temp>0){
				h = 1 ;
			}else{
				h = -1;
			}
			if(h == data[number][4]){
				take = false;
			}else{
				w[0] = w[0] + data[number][4] * 1;
				w[1] = w[1] + data[number][4] * data[number][0] ;
				w[2] = w[2] + data[number][4] * data[number][1] ;
				w[3] = w[3] + data[number][4] * data[number][2] ;
				w[4] = w[4] + data[number][4] * data[number][3] ;
				countstop = 0;
				count++;
				take = true;
			}
			start++;
			if(start==500){
				start = 0;
			}
			if(count!=100){
				again = true;
			}
			
		}
		for(int j=0; j<500; j++){
			double temperr = wpocket[0]*1 + wpocket[1]*test[j][0] +  wpocket[2]*test[j][1] + wpocket[3]*test[j][2] + wpocket[4]*test[j][3] ;
			if(temperr>0){
				h = 1 ;
			}else{
				h = -1;
			}
			if(h == test[j][4]){
			
			}else{
				error = error + 1;
				cerr++;
			}
		}
		totalerr = totalerr + error ;
		his[cerr/5]++;
	}
	
	double answer = totalerr/1000000;
	cout<<answer<<endl;

	//find largest
	int largest=0;
	for(int i=0; i<100; i++){
		if(largest<his[i]){
			largest = his[i] ;
		}
	}

	//set largest
	for(int i=0; i<100; i++){
		his[i] = his[i]*400/largest;
	}

	//draw histogram
	Mat histImg (400,400,CV_8UC3,Scalar(255,255,255));
	for(int i=0; i<100; i++){
		line(histImg,Point(i*16,400-his[i]),Point(i*16,400),Scalar(0,0,0));
	}
	imshow("histogram",histImg);
	imwrite("histogram.jpg",histImg);
	
	waitKey(0);
	system("pause");
	return 0;
}

