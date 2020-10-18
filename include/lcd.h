#ifndef LCD_H_
#define LCD_H_

#include <myhead.h>
#include <font_libs.h>
#include <project.h>
 //  #include <xxx.h> Ĭ��������׼���ͷ�ļ�·��  /usr/include
 //  #include "xxx.h" Ĭ����������ǰ·������������׼���ͷ�ļ�·��  /usr/include
int LCD_fd;
unsigned long *fb_mem;
unsigned char *bmp_buf;
int savebuf[530*5*32];           //����������ʾ���������


 int Init_Lcd(void)
 {
		
		int ret;
		
		//����LCD������ɡ�
		LCD_fd = open("/dev/fb0",O_RDWR);
		if(LCD_fd == -1)
		{
			perror("Open LCD");
			return -1;
		}
	 
		
		//�Դ�ӳ��
       fb_mem = (unsigned long *)mmap(NULL,    //����ռ��ӳ���ַ �� ��ȷ�����Ը�ֵNULL����ϵͳ����
									  FB_SIZE, //ӳ����ڴ�Ĵ�С    LCD 800*480*4
									  PROT_READ |  PROT_WRITE,         //ӳ�����ı�������           PROT_EXEC  Pages may be executed.
																	   //                           PROT_READ  Pages may be read.
																	   //                           PROT_WRITE Pages may be written.
																	   //							PROT_NONE  Pages may not be accessed.
									  MAP_SHARED,       //�����־            MAP_SHARED     MAP_PRIVATE
									  LCD_fd,           //Ŀ���ļ������� ----> �ڴ�ӳ���Ŀ���ļ�
									  0);   //ӳ���ڴ��ƫ�ƴ�С   Ĭ�ϴ�ͷ��ʼ��0
									  
		if(fb_mem == MAP_FAILED)	
		{
			perror("Mmap LCD");
			return -1;	
		}
		
		//Show_Bmp(0,0,"logo.bmp");
		
		return 0;
	 
 }
 
 
 void  UnInit_Lcd(void)
 {
	
	 free(bmp_buf);
	 //����ӳ��
     munmap(fb_mem,   //ӳ���ĵ�ַ��ͨ��mmap���ص�ֵ
		   FB_SIZE); //ӳ��Ĵ�С	
		   
	 close(LCD_fd); 
 }
 
 

 
 int Show_Bmp(int X, int Y,const char *bmpfile)
 {
		FILE *filp;
		int x,y,i,ret;
		int width,height,bits;
		BMPFH  BmpFH;  	 //BMP�ļ���Ϣͷ
		BMPIH  BmpIH;    //BMPλͼ��Ϣͷ
		
		//����һ��BMPλͼ
		// ��BMP����ò������
		filp = fopen(bmpfile,"r");
		if(filp == NULL)
		{
			perror("Fppen BMP");
			return -1;
		}	
		//��ȡBMP���ļ���Ϣͷ  14BYTE
		fread(&BmpFH,sizeof(BMPFH),1,filp); 
		//printf("TYPE: %c",BmpFH.bfType & 0xFF);     //��ʾbfType�ĵ��ֽ�
		//printf("%c \n",(BmpFH.bfType >> 8) & 0xFF); //��ʾbfType�ĸ��ֽ�
		//printf("FILE SIZE: %d \n",BmpFH.bfSize);
		//��ȡBMP��λͼ��Ϣͷ
		fread(&BmpIH,sizeof(BMPIH),1,filp);
		//printf("BIT: %d \n",BmpIH.biBitCount);
		//printf("RGB SIZE: %d \n",BmpIH.biSizeImage);	
		//printf("x width: %d \n",BmpIH.biWidth);
		//printf("x heigth: %d \n",BmpIH.biHeight);
		
		width   = BmpIH.biWidth;
		height  = BmpIH.biHeight;
		bits    = BmpIH.biBitCount>>3;  // �൱�� BmpIH.biBitCount/8 �����һ�����ض�Ӧ���ֽ���
		
		bmp_buf = (unsigned char *)malloc(BmpIH.biSizeImage);
		memset(bmp_buf,0x0,BmpIH.biSizeImage);	
		
		
  		//��ȡBMPλͼ��RGB��Ϣ
		fread(bmp_buf,BmpIH.biSizeImage,1,filp);
		
		//�����ͼƬ��ÿһ�еĴ�С����4�ֽڵı�������ͼƬ���ж����ֽ���䣬�����
		int index;
		int linebyte = (BmpIH.biSizeImage/height); //��ȡ���ݶ���ı�׼�µ�ÿ�����������ֽ���
			
		
		//��24bit��RGB��䵽32bit��ARGB��
		for(index=0,y=(479-Y-height),i=0;y<(479-Y);y++,index+=linebyte)
		{
			for(i=index,x=X;x<X+width;x++,i+=3)
			{				
				fb_mem[(479-y)*800+x]  =  (bmp_buf[i] |  bmp_buf[i+1]<<8  |  bmp_buf[i+2] <<16);
			}

		}
		 // *(fb_mem+(479-y)*800+x)   fb_mem ��Ϊ�Դ���׵�ַ��ͨ��ָ��ƫ�Ʋ��������ʶ�Ӧ������λ��
	      
		//дLCD
		//�ߴ� 7 ��   �ֱ���: 800*480 ɫ�λ���� ARGB  16bit��565; 5:5:5:1��   24bit(8:8:8)  32bit(8:8:8:8)
		// ��ɫ�� 0x00000000   ��ɫ�� 0xFFFFFFFF  ��ɫ��0xFF0000
	 
		fclose(filp);
	 
	 return 0;
 }
 
 
 int Show_Msg(int x,int y,char *msg)
 {
	int i=0;
	char str[50];
	char cn;
	
	while( (cn = *(msg++))!= '\0')
	{
		
		if((cn > 47 && cn < 58) || (cn >96 && cn<123))
		{
			memset(str,0,sizeof(str));
			sprintf(str,"keyboard/%c.bmp",cn);
			Show_Bmp(x+i++*50, y,str);
		}

	}	 
	
	sleep(2);
	return 0;
	 	 
 }
 
 
int  Create_LineEdit(int X,int Y,int width,int height)
{
	int x,y;
	
	for(y=Y;y<Y+height;y++)
	{
		for(x=X;x<X+width;x++)
		{				
			fb_mem[y*800+x]  =  0xFFFFFFFF;
		}

	}	
	
	return 0;		
}


int  Clean_Area(int X,int Y,int width,int height)
{
	int x,y;
	
	for(y=Y;y<Y+height;y++)
	{
		for(x=X;x<X+width;x++)
		{				
			fb_mem[y*800+x]  =  0x00; //���ñ�����ɫΪ��ɫ��������޸�
		}

	}	
	return 0;		
}
int  Clean_Area_w(int X,int Y,int width,int height)
{
	int x,y;
	
	for(y=Y;y<Y+height;y++)
	{
		for(x=X;x<X+width;x++)
		{				
			fb_mem[y*800+x]  =  0xffffff; //���ñ�����ɫΪ��ɫ��������޸�
		}

	}	
	return 0;		
}



int  Draw_Text16(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[])
{
	unsigned short int i,j;
	unsigned char mask,buffer;
	for(i=0;i<16;i++)
	{
		mask =0x80;				//����
		buffer =ch[i*2];		//��ȡһ�еĵ�һ���ֽ�
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
				fb_mem[(y+i)*800+(x+j)]= color;	//Ϊ������ɫ	
			}
			mask =mask >>1;
		}
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
				fb_mem[(y+i)*800+(x+j+8)]= color;
			}
			mask =mask>>1;	
		}		
	}
	
	return 0;
}


int  Draw_TextX(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[],int size)
{
	unsigned short int i,j,k,m;
	unsigned char mask,buffer;
	for(i=0;i<16;i++)
	{
		mask =0x80;				//����
		buffer =ch[i*2];		//��ȡһ�еĵ�һ���ֽ�
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
			  for(k=0;k<size;k++)
			  	{
			  		for(m=0;m<size;m++)
			  		{
					  savebuf[(y+i*size+m)*530*5+(x+j*size+k)]= color;
			  		}
			  	}
			}
			mask =mask >>1;
		}
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
			  for(k=0;k<size;k++)
			  	{	
			  		for(m=0;m<size;m++)
			  		{	
					   savebuf[(y+i*size+m)*530*5+(x+(j+8)*size+k)]= color;
			  		}
			  	}
			}
			mask =mask>>1;	
		}		
	}
	
	return 0;
}

int  Draw_TextXX(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[],int size)
{
	unsigned short int i,j,k,m;
	unsigned char mask,buffer;
	for(i=0;i<16;i++)
	{
		mask =0x80;				//����
		buffer =ch[i*2];		//��ȡһ�еĵ�һ���ֽ�
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
			  for(k=0;k<size;k++)
			  	{
			  		for(m=0;m<size;m++)
			  		{
					  fb_mem[(y+i*size+m)*800+(x+j*size+k)]= color;
			  		}
			  	}
			}
			mask =mask >>1;
		}
		mask =0x80;
		buffer =ch[i*2+1];
		for(j=0;j<8;j++)
		{
			if(buffer &mask)
			{
			  for(k=0;k<size;k++)
			  	{	
			  		for(m=0;m<size;m++)
			  		{	
					   fb_mem[(y+i*size+m)*800+(x+(j+8)*size+k)]= color;
			  		}
			  	}
			}
			mask =mask>>1;	
		}		
	}
	
	return 0;
}

int  Draw_ASCII(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[])
{
       unsigned short int i,j;
       unsigned char mask,buffer;
      
       for(i=0;i<16;i++)
       {
              mask=0x80;
              buffer=ch[i];
              for(j=0;j<8;j++)
              {                   
                     if(buffer&mask)
                     {
                            fb_mem[(y+i)*800+(x+j)]= color;
                     }
                     mask=mask>>1;                   
              }
       }
	   
	   return 0;
}


int  Draw_ASCIIX(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[],int size)
{
       unsigned short int i,j,k,m;
       unsigned char mask,buffer;
      
       for(i=0;i<16;i++)
       {
              mask=0x80;
              buffer=ch[i];
              for(j=0;j<8;j++)
              {                   
                     if(buffer&mask)
                     {
						 for(k=0;k<size;k++)
						   {
							   for(m=0;m<size;m++)
							   {
							      savebuf[(y+i*size+m)*530*5+(x+j*size+k)]= color;
							   }
						   }

                     }
                     mask=mask>>1;                   
              }
       }
	   
	   return 0;
}

int  Draw_ASCIIXX(unsigned int x,unsigned int y,unsigned long color,const unsigned char ch[],int size)
{
       unsigned short int i,j,k,m;
       unsigned char mask,buffer;
      
       for(i=0;i<16;i++)
       {
              mask=0x80;
              buffer=ch[i];
              for(j=0;j<8;j++)
              {                   
                     if(buffer&mask)
                     {
						 for(k=0;k<size;k++)
						   {
							   for(m=0;m<size;m++)
							   {
							      fb_mem[(y+i*size+m)*800+(x+j*size+k)]= color;
							   }
						   }

                     }
                     mask=mask>>1;                   
              }
       }
	   
	   return 0;
}

int  Display_character(unsigned int x,unsigned int y,unsigned int len,unsigned char *string)
{
	int k, xx;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length-1;k++)
	{
		if(string[k]&0x80)   //�����ַ�
		{
			qh =string[k]-0xa0;			//����
			wh =string[k+1]-0xa0;		//λ��
			mould =&__MYCHS[((qh-1)*94+wh-1)*32];
			Draw_Text16(4+xx,y,0xFF0000,mould);
			xx+=16;
			k++;
		}
		else
		{
			mould =&__ASCII[string[k]*16];
			Draw_ASCII(4+xx,y,0xFF0000,mould);
			xx+=8;
		}
	}
	return 0;
}

int  Display_characterX(unsigned int x,unsigned int y,unsigned int len,unsigned char *string,int size)
{
	int k, xx,yy=y;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length;k++)
	{
		if(string[k]&0x80)   //�����ַ�
		{	
			qh =string[k]-0xa0;			//����
			wh =string[k+1]-0xa0;		//λ��
			mould =&__MYCHS[((qh-1)*94+wh-1)*32];
			Draw_TextX(4*size+xx,yy,0xff,mould,size); //��4Ϊ����ÿ������֮����һ���ļ�϶
			xx+=16*size;//��ǰ��������ģΪ32*32,ÿ����ʾ��һ������ʱ����ƫ��32��bit
			k++; //������һ�����ģ�����++������ƫ��2���ֽ�
		}
		else     //�������ַ�
		{	
			mould =&__ASCII[string[k]*16];
			Draw_ASCIIX(4*size+xx,yy,0xff,mould,size);
			xx+=8*size;//��ǰ��ASCII��ģ��ʾΪ8*16,ÿ����ʾ��һ������ʱ����ƫ��8��bit
		}
	}
	turnlen = xx;
	return 0;
}

int  Display_characterXX(unsigned int x,unsigned int y,unsigned int len,unsigned char *string,int size,unsigned long color)
{
	int k, xx,yy=y;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length;k++)
	{
		if(string[k]&0x80)   //�����ַ�
		{
			if(xx>800-16)
			{
				xx=x;
				yy=y+size*16;
			}	
			qh =string[k]-0xa0;			//����
			wh =string[k+1]-0xa0;		//λ��
			mould =&__MYCHS[((qh-1)*94+wh-1)*32];
			Draw_TextXX(4*size+xx,yy,color,mould,size); //��4Ϊ����ÿ������֮����һ���ļ�϶
			xx+=16*size;//��ǰ��������ģΪ32*32,ÿ����ʾ��һ������ʱ����ƫ��32��bit
			k++; //������һ�����ģ�����++������ƫ��2���ֽ�
		}
		else     //�������ַ�
		{
			if(xx>800-8)
			{
				xx=x;
				yy=y+size*16;
			}	
			mould =&__ASCII[string[k]*16];
			Draw_ASCIIXX(4*size+xx,yy,color,mould,size);
			xx+=8*size;//��ǰ��ASCII��ģ��ʾΪ8*16,ÿ����ʾ��һ������ʱ����ƫ��8��bit
		}
	}
	x_x=xx;
	y_y=yy;
	return 0;
}

int  Display_characterXXX(unsigned int x,unsigned int y,unsigned int len,unsigned char *string,int size,unsigned long color)
{
	int k, xx,yy=y;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length;k++)
	{
		if(string[k]&0x80)   //�����ַ�
		{
			if(xx>800-16)
			{
				xx=x;
				yy=yy+size*16;
			}	
			qh =string[k]-0xa0;			//����
			wh =string[k+1]-0xa0;		//λ��
			mould =&__MYCHS[((qh-1)*94+wh-1)*32];
			Draw_TextXX(4*size+xx,yy,color,mould,size); //��4Ϊ����ÿ������֮����һ���ļ�϶
			xx+=16*size;//��ǰ��������ģΪ32*32,ÿ����ʾ��һ������ʱ����ƫ��32��bit
			k++; //������һ�����ģ�����++������ƫ��2���ֽ�
		}
		else     //�������ַ�
		{
			if(xx>800-8)
			{
				xx=x;
				yy=yy+size*16;
			}	
			mould =&__ASCII[string[k]*16];
			Draw_ASCIIXX(4*size+xx,yy,color,mould,size);
			xx+=8*size;//��ǰ��ASCII��ģ��ʾΪ8*16,ÿ����ʾ��һ������ʱ����ƫ��8��bit
		}
	}
	return 0;
}

int  Display_characterXXXX(unsigned int x,unsigned int y,unsigned int len,unsigned char *string,int size,unsigned long color)
{
	int k, xx,yy=y;
	unsigned char qh,wh;
	const unsigned char *mould;
	unsigned int length =len;
	
	for(k=0,xx=x;k<length;k++)
	{
		if(string[k]&0x80)   //�����ַ�
		{
			if(xx>=600-16)
			{
				xx=200;
				yy=yy+size*16;
			}	
			qh =string[k]-0xa0;			//����
			wh =string[k+1]-0xa0;		//λ��
			mould =&__MYCHS[((qh-1)*94+wh-1)*32];
			Draw_TextXX(4*size+xx,yy,color,mould,size); //��4Ϊ����ÿ������֮����һ���ļ�϶
			xx+=16*size;//��ǰ��������ģΪ32*32,ÿ����ʾ��һ������ʱ����ƫ��32��bit
			k++; //������һ�����ģ�����++������ƫ��2���ֽ�
		}
		else     //�������ַ�
		{
			if(xx>=600-8)
			{
				xx=200;
				yy=yy+size*16;
			}	
			mould =&__ASCII[string[k]*16];
			Draw_ASCIIXX(4*size+xx,yy,color,mould,size);
			xx+=8*size;//��ǰ��ASCII��ģ��ʾΪ8*16,ÿ����ʾ��һ������ʱ����ƫ��8��bit
		}
	}
	return 0;
}
int Roll_Dispaly(unsigned char *str)
{
	int x;
	
	for(x=600;x>20;x--)
	{
	  Display_characterXX(x,45,strlen(str)+1,str,4,0xFF0000);
	  usleep(50000);
	  Display_characterXX(x+1,45,strlen(str)+1,str,4,0xFFFFFFFF); 
    }
	
	Display_characterXX(x+1,45,strlen(str)+1,str,4,0xFFFFFFFF);
	return 0;
}

#endif