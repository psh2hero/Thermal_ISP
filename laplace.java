public static Bitmap LaplaceEdgeDetection(Bitmap Image)
 
{
 
    try
 
    {
 
        using (Bitmap TempImage = ConvertBlackAndWhite(Image))
 
        {
 
            Filter TempFilter = new Filter(5, 5);
 
            TempFilter.MyFilter[0, 0] = -1;
 
            TempFilter.MyFilter[0, 1] = -1;
 
            TempFilter.MyFilter[0, 2] = -1;
 
            TempFilter.MyFilter[0, 3] = -1;
 
            TempFilter.MyFilter[0, 4] = -1;
 
            TempFilter.MyFilter[1, 0] = -1;
 
            TempFilter.MyFilter[1, 1] = -1;
 
            TempFilter.MyFilter[1, 2] = -1;
 
            TempFilter.MyFilter[1, 3] = -1;
 
            TempFilter.MyFilter[1, 4] = -1;
 
            TempFilter.MyFilter[2, 0] = -1;
 
            TempFilter.MyFilter[2, 1] = -1;
 
            TempFilter.MyFilter[2, 2] = 24;
 
            TempFilter.MyFilter[2, 3] = -1;
 
            TempFilter.MyFilter[2, 4] = -1;
 
            TempFilter.MyFilter[3, 0] = -1;
 
            TempFilter.MyFilter[3, 1] = -1;
 
            TempFilter.MyFilter[3, 2] = -1;
 
            TempFilter.MyFilter[3, 3] = -1;
 
            TempFilter.MyFilter[3, 4] = -1;
 
            TempFilter.MyFilter[4, 0] = -1;
 
            TempFilter.MyFilter[4, 1] = -1;
 
            TempFilter.MyFilter[4, 2] = -1;
 
            TempFilter.MyFilter[4, 3] = -1;
 
            TempFilter.MyFilter[4, 4] = -1;
 
            using (Bitmap NewImage = TempFilter.ApplyFilter(TempImage))
 
            {
 
                return Negative(NewImage);
 
            }
 
        }
 
    }
 
    catch { throw; }
 
}