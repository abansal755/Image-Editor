#include<fstream>
#include<QImage>
#include<QDebug>
#include<math.h>
using namespace std;

void blur(QImage&img,int radius=1){
    int width = img.width(), height = img.height();
        QImage img1(width,height,img.format());
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                int r = 0, g = 0, b = 0;
                int num = 0;
                for (int di = -radius; di <= radius; di++) {
                    for (int dj = -radius; dj <= radius; dj++) {
                        if (i + di < 0 || i + di >= height) continue;
                        if (j + dj < 0 || j + dj >= width) continue;
                        QColor c=img.pixelColor(j+dj,i+di);
                        r += c.red();
                        g += c.green();
                        b += c.blue();
                        num++;
                    }
                }
                r /= num;
                g /= num;
                b /= num;
                img1.setPixelColor(j,i,qRgb(r,g,b));
            }
        }
        img=img1;
}

void saturate(QImage&img,float x){
    for(int i=0;i<img.height();i++){
        for(int j=0;j<img.width();j++){
             QColor c=img.pixelColor(j,i);
             float h=c.hueF();
             float s=c.saturationF();
             float v=c.valueF();
             if(x>=0) s+=(1-s)*x;
             else s*=1-abs(x);
             c.setHsvF(h,s,v);
             img.setPixelColor(j,i,c);
        }
    }
}


void gammaCorrect(QImage&image,float gamma){
    for(int y=0;y<image.height();y++){
        for(int x=0;x<image.width();x++){
            QColor c=image.pixelColor(x,y);
            c.setRed(255*pow((float)c.red()/255,1/gamma));
            c.setGreen(255*pow((float)c.green()/255,1/gamma));
            c.setBlue(255*pow((float)c.blue()/255,1/gamma));
            image.setPixelColor(x,y,c);
        }
    }
}

void grade(QImage&image, float lift,float gain,float offset){
    for(int y=0;y<image.height();y++){
        for(int x=0;x<image.width();x++){
            QColor c=image.pixelColor(x,y);
            float r=c.redF();
            float g=c.greenF();
            float b=c.blueF();
            r=(r*(gain-lift))+lift+offset;
            g=(g*(gain-lift))+lift+offset;
            b=(b*(gain-lift))+lift+offset;
            if(r<0) r=0;
            if(r>1) r=1;
            if(g<0) g=0;
            if(g>1) g=1;
            if(b<0) b=0;
            if(b>1) b=1;
            c.setRedF(r);
            c.setGreenF(g);
            c.setBlueF(b);
            image.setPixelColor(x,y,c);
        }
    }
}

void rotateClockwise90(QImage&img) {
    QImage img1(img.height(),img.width(),img.format());
    for(int y=0;y<img.height();y++){
        for(int x=0;x<img.width();x++){
            img1.setPixelColor(img.height()-y-1,x,img.pixelColor(x,y));
        }
    }
    img=img1;
}

void rotateAntiClockwise90(QImage&img){
    QImage img1(img.height(),img.width(),img.format());
    for(int y=0;y<img.height();y++){
        for(int x=0;x<img.width();x++){
            img1.setPixelColor(y,img.width()-x-1,img.pixelColor(x,y));
        }
    }
    img=img1;
}


void rotate180(QImage&img){
    QImage img1(img.width(),img.height(),img.format());
    for(int y=0;y<img.height();y++){
        for(int x=0;x<img.width();x++){
            img1.setPixelColor(img.width()-1-x,img.height()-1-y,img.pixelColor(x,y));
        }
    }
    img=img1;
}
