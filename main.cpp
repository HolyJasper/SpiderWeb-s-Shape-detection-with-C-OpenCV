#include <opencv2/opencv.hpp>
#include <bits/stdc++.h>

using namespace cv;
using namespace std;

int main()
{
    Mat web = imread("./realWeb.png", IMREAD_GRAYSCALE); // single channel image, black and white
    imwrite("./single.png", web);
    if (web.empty())
    {
        cout << "ERROR: Fail to read the image\n";
        return -1;
    }

    Mat thresholdDst;
    // transform the image into real black and real white
    threshold(web, thresholdDst, 188, 255, THRESH_BINARY);
    imwrite("./threshold.png", thresholdDst);

    // do the blur to make it smooth
    Mat blurImage;
    GaussianBlur(thresholdDst, blurImage, Size(5, 5), 0, 0);
    imwrite("./blur.png", blurImage);

    // find the outlines
    Mat contour = Mat::zeros(blurImage.size(), CV_8UC3);
    Scalar blue = Scalar(255, 255, 100);
    vector<vector<Point>> outLines;
    findContours(blurImage, outLines, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // RETR_EXTERNAL means the outside only

    for (auto i = 0; i < outLines.size(); i++)
    {
        drawContours(contour, outLines, i, blue, 3);
    }
    imwrite("./contour.png", contour);
    // test canny

    // find the largest outlines
    double Area = 0;
    vector<Point> webArea;
    for (const auto &area : outLines)
    {
        double present = contourArea(area);
        if (present > Area)
        {
            Area = present;
            webArea = area;
        }
    }
    // find the web
    Mat web1 = Mat::zeros(blurImage.size(), CV_8UC3);
    drawContours(web1, vector<vector<Point>>{webArea}, -1, blue, 5);
    imwrite("./web1.png", web1);

    Rect rect = boundingRect(webArea);
    double ratio = double(rect.width) / double(rect.height);
    double peri = arcLength(webArea, true);
    approxPolyDP(webArea, webArea, 0.02 * peri, true);

    Mat web2 = Mat::zeros(blurImage.size(), CV_8UC3);
    drawContours(web2, vector<vector<Point>>{webArea}, -1, blue, 5);
    imwrite("./web2.png", web2);

    int objSize = webArea.size();

    cout << "The outLines number are : " << objSize << "\n";

    return 0;
}
