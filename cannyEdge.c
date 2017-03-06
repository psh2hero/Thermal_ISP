void CannyEdge(BYTE *pImage, int width, int height, int th_high, int th_low, BYTE *pEdge)
{
    int i, j;
 
    int dx, dy, mag, slope, direction;
    int index, index2;
 
    const int fbit = 10;
    const int tan225 =   424;       // tan25.5 << fbit, 0.4142
    const int tan675 =   2472;      // tan67.5 << fbit, 2.4142
     
    const int CERTAIN_EDGE = 255;
    const int PROBABLE_EDGE = 100;
 
    bool bMaxima;
     
    int *mag_tbl = new int[width*height];
    int *dx_tbl = new int[width*height];
    int *dy_tbl = new int[width*height];
 
    BYTE **stack_top, **stack_bottom;
    stack_top = new BYTE*[width*height];
    stack_bottom = stack_top;
#define CANNY_PUSH(p)    *(p) = CERTAIN_EDGE, *(stack_top++) = (p)
#define CANNY_POP()      *(--stack_top)
 
    for(i=0; i<width*height; i++) {
        mag_tbl[i] = 0;
        pEdge[i] = 0;
    }
 
    // Sobel Edge Detection
    for(i=1; i<height-1; i++) {
        index = i*width;
        for(j=1; j<width-1; j++) {
            index2 = index+j;
            // -1 0 1
            // -2 0 2
            // -1 0 1
            dx = pImage[index2-width+1] + (pImage[index2+1]<<1) + pImage[index2+width+1]
                   -pImage[index2-width-1] - (pImage[index2-1]<<1) - pImage[index2+width-1];
 
            // -1 -2 -1
            //  0  0  0
            //  1  2  1
            dy = -pImage[index2-width-1] - (pImage[index2-width]<<1) - pImage[index2-width+1]
                   +pImage[index2+width-1] + (pImage[index2+width]<<1) + pImage[index2+width+1];
 
             mag = abs(dx)+abs(dy);     // magnitude
            //mag = sqrtf(dx*dx + dy*dy);
 
            dx_tbl[index2] = dx;
            dy_tbl[index2] = dy;
            mag_tbl[index2] = mag;              // store the magnitude in the table
        }   // for(j)
    }   // for(i)
     
    for(i=1; i<height-1; i++) {
        index = i*width;
        for(j=1; j<width-1; j++) {
            index2 = index+j;
 
            mag = mag_tbl[index2];              // retrieve the magnitude from the table
 
            // if the magnitude is greater than the lower threshold
            if(mag > th_low) {
                             
                // determine the orientation of the edge
                dx = dx_tbl[index2];
                dy = dy_tbl[index2];
 
                if(dx != 0) {
                    slope = (dy<<fbit)/dx;
 
                    if(slope > 0) {
                        if(slope < tan225)
                            direction = 0;
                        else if(slope < tan675)
                            direction = 1;
                        else
                            direction = 2;
                    }
                    else {
                        if(-slope > tan675)
                            direction = 2;
                        else if(-slope > tan225)
                            direction = 3;
                        else
                            direction = 0;
                    }
                }
                else
                    direction = 2;
             
                bMaxima = true;
                // perform non-maxima suppression
                if(direction == 0) {
                    if(mag < mag_tbl[index2-1] || mag < mag_tbl[index2+1])
                        bMaxima = false;
                }
                else if(direction == 1) {
                    if(mag < mag_tbl[index2+width+1] || mag < mag_tbl[index2-width-1])
                        bMaxima = false;
                }
                else if(direction == 2){
                    if(mag < mag_tbl[index2+width] || mag < mag_tbl[index2-width])
                        bMaxima = false;
                }
                else { // if(direction == 3)
                    if(mag < mag_tbl[index2+width-1] || mag < mag_tbl[index2-width+1])
                        bMaxima = false;
                }
 
                if(bMaxima) {
                    if(mag > th_high) {
                        pEdge[index2] = CERTAIN_EDGE;           // the pixel does belong to an edge
                        *(stack_top++) = (BYTE*)(pEdge+index2);
                    }
                    else
                        pEdge[index2] = PROBABLE_EDGE;          // the pixel might belong to an edge
                }
            }
             
        }   // for(j)
    }   // for(i)
 
    while(stack_top != stack_bottom) {
        BYTE* p = CANNY_POP();
 
        if(*(p+1) == PROBABLE_EDGE)
            CANNY_PUSH(p+1);
        if(*(p-1) == PROBABLE_EDGE)
            CANNY_PUSH(p-1);
        if(*(p+width) == PROBABLE_EDGE)
            CANNY_PUSH(p+width);
        if(*(p-width) == PROBABLE_EDGE)
            CANNY_PUSH(p-width);
        if(*(p-width-1) == PROBABLE_EDGE)
            CANNY_PUSH(p-width-1);
        if(*(p-width+1) == PROBABLE_EDGE)
            CANNY_PUSH(p-width+1);
        if(*(p+width-1) == PROBABLE_EDGE)
            CANNY_PUSH(p+width-1);
        if(*(p+width+1) == PROBABLE_EDGE)
            CANNY_PUSH(p+width+1);
    }
 
    for(i=0; i<width*height; i++)
        if(pEdge[i]!=CERTAIN_EDGE)
            pEdge[i] = 0;
 
    delete [] mag_tbl;
    delete [] dx_tbl;
    delete [] dy_tbl;
    delete [] stack_bottom;
}