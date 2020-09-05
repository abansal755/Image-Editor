//#include<fstream>
//#include<QImage>
//#include<QDebug>
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
