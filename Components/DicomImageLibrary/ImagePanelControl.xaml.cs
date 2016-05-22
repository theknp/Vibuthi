﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;

namespace DicomImageLibrary
{
    /// <summary>
    /// Interaction logic for ImagePanelControl.xaml
    /// </summary>
    public partial class ImagePanelControl : System.Windows.Controls.UserControl
    {

        public enum ImageBitsPerPixel { Eight, Sixteen, TwentyFour };
        public enum ViewSettings { Zoom1_1, ZoomToFit };

        List<byte> pix8;
        List<ushort> pix16;
        List<byte> pix24;
        Bitmap bmp;
        int hOffset;
        int vOffset;
        int hMax;
        int vMax;
        int imgWidth;
        int imgHeight;
        int panWidth;
        int panHeight;
        bool newImage;

        // For Window Level
        int winMin;
        int winMax;
        int winCentre;
        int winWidth;
        int winWidthBy2;
        int deltaX;
        int deltaY;

        System.Windows.Point ptWLDown;
        double changeValWidth;
        double changeValCentre;
        bool rightMouseDown;
        bool imageAvailable;
        bool signed16Image;

        byte[] lut8;
        byte[] lut16;
        byte[] imagePixels8;
        byte[] imagePixels16;
        byte[] imagePixels24;
        int sizeImg;
        int sizeImg3;
        //MainForm mf;

        ImageBitsPerPixel bpp;

        public ViewSettings viewSettings;
        public bool viewSettingsChanged;

        public ImagePanelControl()
        {
            InitializeComponent();
            //DoubleBuffered = true;

            pix8 = new List<byte>();
            pix16 = new List<ushort>();
            pix24 = new List<byte>();

            this.hScrollBar.Visibility = Visibility.Hidden;
            this.vScrollBar.Visibility = Visibility.Hidden;

            winMin = 0;
            winMax = 65535;

            ptWLDown = new System.Windows.Point();
            changeValWidth = 0.5;
            changeValCentre = 20.0;
            rightMouseDown = false;
            imageAvailable = false;
            signed16Image = false;

            lut8 = new byte[256];
            lut16 = new byte[65536];

            viewSettings = ViewSettings.ZoomToFit;
            viewSettingsChanged = false;

            PerformResize();
        }

        public bool NewImage
        {
            set
            {
                newImage = value;
            }
        }

        public bool Signed16Image
        {
            set { signed16Image = value; }
        }

        public void SetParameters(ref List<byte> arr, int wid, int hei, double windowWidth,
            double windowCentre, int samplesPerPixel, bool resetScroll)//, MainForm mainFrm
        {
            if (samplesPerPixel == 1)
            {
                bpp = ImageBitsPerPixel.Eight;
                imgWidth = wid;
                imgHeight = hei;
                winWidth = Convert.ToInt32(windowWidth);
                winCentre = Convert.ToInt32(windowCentre);
                changeValWidth = 0.1;
                changeValCentre = 20.0;
                sizeImg = imgWidth * imgHeight;
                sizeImg3 = sizeImg * 3;

                pix8 = arr;
                imagePixels8 = new byte[sizeImg3];

                //mf = mainFrm;
                imageAvailable = true;
                if (bmp != null)
                    bmp.Dispose();
                ResetValues();
                ComputeLookUpTable8();
                bmp = new Bitmap(imgWidth, imgHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                CreateImage8();
            }

            if (samplesPerPixel == 3)
            {
                bpp = ImageBitsPerPixel.TwentyFour;
                imgWidth = wid;
                imgHeight = hei;
                winWidth = Convert.ToInt32(windowWidth);
                winCentre = Convert.ToInt32(windowCentre);
                changeValWidth = 0.1;
                changeValCentre = 0.1;
                sizeImg = imgWidth * imgHeight;
                sizeImg3 = sizeImg * 3;

                pix24 = arr;
                imagePixels24 = new byte[sizeImg3];

                //mf = mainFrm;
                imageAvailable = true;
                if (bmp != null)
                    bmp.Dispose();
                ResetValues();
                ComputeLookUpTable8();
                bmp = new Bitmap(imgWidth, imgHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                CreateImage24();
            }
            if (resetScroll == true) ComputeScrollBarParameters();
            InvalidateVisual();
        }

        void DetermineMouseSensitivity()
        {
            // Modify the 'sensitivity' of the mouse based on the current window width
            if (winWidth < 10)
            {
                changeValWidth = 0.1;
            }
            else if (winWidth >= 20000)
            {
                changeValWidth = 40;
            }
            else
            {
                changeValWidth = 0.1 + (winWidth - 10) / 500.0;
            }

            changeValCentre = changeValWidth;
        }

        public void SetParameters(ref List<ushort> arr, int wid, int hei, double windowWidth,
            double windowCentre, bool resetScroll)//, MainForm mainFrm
        {
            bpp = ImageBitsPerPixel.Sixteen;
            imgWidth = wid;
            imgHeight = hei;
            winWidth = Convert.ToInt32(windowWidth);
            winCentre = Convert.ToInt32(windowCentre);

            sizeImg = imgWidth * imgHeight;
            sizeImg3 = sizeImg * 3;
            double sizeImg3By4 = sizeImg3 / 4.0;

            DetermineMouseSensitivity();

            pix16 = arr;
            imagePixels16 = new byte[sizeImg3];

            //mf = mainFrm;
            imageAvailable = true;
            if (bmp != null)
                bmp.Dispose();
            ResetValues();
            ComputeLookUpTable16();
            bmp = new Bitmap(imgWidth, imgHeight, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
            CreateImage16();
            if (resetScroll == true) ComputeScrollBarParameters();
            InvalidateVisual();
        }

        // Create a bitmap on the fly, using 8-bit grayscale pixel data
        private void CreateImage8()
        {
            BitmapData bmd = bmp.LockBits(new System.Drawing.Rectangle(0, 0, imgWidth, imgHeight),
                System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);

            unsafe
            {
                int pixelSize = 3;
                int i, j, j1, i1;
                byte b;

                for (i = 0; i < bmd.Height; ++i)
                {
                    byte* row = (byte*)bmd.Scan0 + (i * bmd.Stride);
                    i1 = i * bmd.Width;

                    for (j = 0; j < bmd.Width; ++j)
                    {
                        b = lut8[pix8[i * bmd.Width + j]];
                        j1 = j * pixelSize;
                        row[j1] = b;            // Red
                        row[j1 + 1] = b;        // Green
                        row[j1 + 2] = b;        // Blue
                    }
                }
            }
            bmp.UnlockBits(bmd);
        }

        // Create a bitmap on the fly, using 24-bit RGB pixel data
        private void CreateImage24()
        {
            {
                int numBytes = imgWidth * imgHeight * 3;
                int j;
                int i, i1;

                BitmapData bmd = bmp.LockBits(new System.Drawing.Rectangle(0, 0, bmp.Width,
                    bmp.Height), ImageLockMode.WriteOnly, bmp.PixelFormat);

                int width3 = bmd.Width * 3;

                unsafe
                {
                    for (i = 0; i < bmd.Height; ++i)
                    {
                        byte* row = (byte*)bmd.Scan0 + (i * bmd.Stride);
                        i1 = i * bmd.Width * 3;

                        for (j = 0; j < width3; j += 3)
                        {
                            // Windows uses little-endian, so the RGB data is 
                            //  actually stored as BGR
                            row[j + 2] = lut8[pix24[i1 + j]];     // Blue
                            row[j + 1] = lut8[pix24[i1 + j + 1]]; // Green
                            row[j] = lut8[pix24[i1 + j + 2]];     // Red
                        }
                    }
                }
                bmp.UnlockBits(bmd);
            }
        }

        // Create a bitmap on the fly, using 16-bit grayscale pixel data
        private void CreateImage16()
        {
            BitmapData bmd = bmp.LockBits(new System.Drawing.Rectangle(0, 0, imgWidth, imgHeight),
               System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);

            unsafe
            {
                int pixelSize = 3;
                int i, j, j1, i1;
                byte b;

                for (i = 0; i < bmd.Height; ++i)
                {
                    byte* row = (byte*)bmd.Scan0 + (i * bmd.Stride);
                    i1 = i * bmd.Width;

                    for (j = 0; j < bmd.Width; ++j)
                    {
                        b = lut16[pix16[i * bmd.Width + j]];
                        j1 = j * pixelSize;
                        row[j1] = b;            // Red
                        row[j1 + 1] = b;        // Green
                        row[j1 + 2] = b;        // Blue
                    }
                }
            }
            bmp.UnlockBits(bmd);
        }

        private void ComputeScrollBarParameters()
        {
            panWidth = (int)this.Width;
            panHeight = (int)this.Height;

            hOffset = (panWidth - imgWidth) / 2;
            vOffset = (panHeight - imgHeight) / 2;

            if (imgWidth < panWidth)
            {
                hScrollBar.Visibility = Visibility.Hidden;
            }
            else
            {
                hScrollBar.Visibility = Visibility.Visible;
                hScrollBar.Value = (hScrollBar.Maximum + 1 -
                    hScrollBar.LargeChange - hScrollBar.Minimum) / 2;
            }

            if (imgHeight < panHeight)
            {
                vScrollBar.Visibility = Visibility.Hidden;
            }
            else
            {
                vScrollBar.Visibility = Visibility.Visible;
                vScrollBar.Value = (vScrollBar.Maximum + 1 -
                    vScrollBar.LargeChange - vScrollBar.Minimum) / 2;
            }
        }

        private void vScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            int val = (int)vScrollBar.Value;
            vOffset = (panHeight - imgHeight) * (val - (int)vScrollBar.Minimum) /
                    (vMax - (int)vScrollBar.Minimum);
            InvalidateVisual();
        }

        private void hScrollBar_Scroll(object sender, ScrollEventArgs e)
        {
            int val = (int)hScrollBar.Value;
            hOffset = (panWidth - imgWidth) * (val - (int)hScrollBar.Minimum) /
                (hMax - (int)hScrollBar.Minimum);
            InvalidateVisual();
        }

        void SetScrollVisibility()
        {
            if (imgWidth >= panWidth)
            {
                hScrollBar.Visibility = Visibility.Visible;
            }

            if (imgHeight >= panHeight)
            {
                vScrollBar.Visibility = Visibility.Visible;
            }
        }

        private void ImagePanel_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
        {
            Bitmap newimg = new Bitmap(300,300);
            Graphics g = Graphics.FromImage(newimg);
            g.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBilinear;

            if (viewSettingsChanged || newImage)
                g.Clear(System.Drawing.SystemColors.Control);

            if (((bpp == ImageBitsPerPixel.Eight) && (pix8.Count > 0)) ||
                 ((bpp == ImageBitsPerPixel.Sixteen) && (pix16.Count > 0)) ||
                 ((bpp == ImageBitsPerPixel.TwentyFour) && (pix24.Count > 0)))
            {
                if (viewSettings == ViewSettings.Zoom1_1)
                {
                    SetScrollVisibility();
                    g.DrawImage(bmp, hOffset, vOffset);
                }
                else // if(viewSettings == ViewSettings.ZoomToFit)
                {
                    ScaleImageKeepingAspectRatio(ref g, bmp, panWidth, panHeight);
                }
            }
            g.Dispose();
            viewSettingsChanged = false;
            newImage = false;
        }

        // Adapted from Charles Petzold's code from his book: Programming Microsoft Windows With C#, 2001
        //  Chapter on Images and Bitmaps - method ScaleImageIsotropically
        //  Code for this available on Petzold's website at 
        //  http://www.charlespetzold.com/books.html
        //  Modifications in July 2012
        void ScaleImageKeepingAspectRatio(ref Graphics grfx, System.Drawing.Image image, int panelWidth, int panelHeight)
        {
            hScrollBar.Visibility = Visibility.Hidden;
            vScrollBar.Visibility = Visibility.Hidden;
            SizeF sizef = new SizeF(image.Width / image.HorizontalResolution,
                                    image.Height / image.VerticalResolution);

            float fScale = Math.Min(panelWidth / sizef.Width,
                                    panelHeight / sizef.Height);

            sizef.Width *= fScale;
            sizef.Height *= fScale;

            grfx.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            grfx.DrawImage(image, (panelWidth - sizef.Width) / 2,
                                  (panelHeight - sizef.Height) / 2,
                                  sizef.Width, sizef.Height);
        }

        // Method to save an image as PNG. The image is saved as per the current window/level values.
        public void SaveImage(String fileName)
        {
            if (bmp != null)
                bmp.Save(fileName, ImageFormat.Png);
        }

        // We use the linear interpolation method here
        //  Nonlinear methods like sigmoid are also common, but we don't do them here.
        private void ComputeLookUpTable8()
        {
            if (winMax == 0)
                winMax = 255;

            int range = winMax - winMin;
            if (range < 1) range = 1;
            double factor = 255.0 / range;

            for (int i = 0; i < 256; ++i)
            {
                if (i <= winMin)
                    lut8[i] = 0;
                else if (i >= winMax)
                    lut8[i] = 255;
                else
                {
                    lut8[i] = (byte)((i - winMin) * factor);
                }
            }
        }

        // Linear interpolation here too
        private void ComputeLookUpTable16()
        {
            int range = winMax - winMin;
            if (range < 1) range = 1;
            double factor = 255.0 / range;
            int i;

            for (i = 0; i < 65536; ++i)
            {
                if (i <= winMin)
                    lut16[i] = 0;
                else if (i >= winMax)
                    lut16[i] = 255;
                else
                {
                    lut16[i] = (byte)((i - winMin) * factor);
                }
            }
        }

        private void panel_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (imageAvailable == true)
            {
                if (e.Button == MouseButtons.Right)
                {
                    ptWLDown.X = e.X;
                    ptWLDown.Y = e.Y;
                    rightMouseDown = true;
                    Cursor = System.Windows.Input.Cursors.Hand;
                }
            }
        }

        // Mouse-move is made to perform window-level
        private void panel_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            DetermineMouseSensitivity();
            if (rightMouseDown == true)
            {
                winWidthBy2 = winWidth / 2;
                winWidth = winMax - winMin;
                winCentre = winMin + winWidthBy2;

                deltaX = (int)((ptWLDown.X - e.X) * changeValWidth);
                deltaY = (int)((ptWLDown.Y - e.Y) * changeValCentre);

                winCentre -= deltaY;
                winWidth -= deltaX;

                if (winWidth < 2) winWidth = 2;
                winWidthBy2 = winWidth / 2;

                winMax = winCentre + winWidthBy2;
                winMin = winCentre - winWidthBy2;

                if (winMin >= winMax) winMin = winMax - 1;
                if (winMax <= winMin) winMax = winMin + 1;

                ptWLDown.X = e.X;
                ptWLDown.Y = e.Y;

                UpdateMainForm();
                if (bpp == ImageBitsPerPixel.Eight)
                {
                    ComputeLookUpTable8();
                    CreateImage8();
                }
                else if (bpp == ImageBitsPerPixel.Sixteen)
                {
                    ComputeLookUpTable16();
                    CreateImage16();
                }
                else // (bpp == ImageBitsPerPixel.TwentyFour)
                {
                    ComputeLookUpTable8();
                    CreateImage24();
                }

                InvalidateVisual();
            }
        }

        private void panel_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e)
        {
            if (rightMouseDown == true)
            {
                rightMouseDown = false;
                Cursor = System.Windows.Input.Cursors.Arrow;
            }
        }

        // Update the graph control on the main form
        private void UpdateMainForm()
        {
            //mf.UpdateWindowLevel(winWidth, winCentre, bpp);
        }

        // Restore original window/level values
        public void ResetValues()
        {
            winMax = Convert.ToInt32(winCentre + 0.5 * winWidth);
            winMin = winMax - winWidth;
            UpdateMainForm();
        }

        private void ImagePanelControl_Resize(object sender, EventArgs e)
        {
            PerformResize();
        }

        private void PerformResize()
        {
            //RenderSize.Width.Width = ClientRectangle.Width - 24;
            //this.Height = ClientRectangle.Height - 24;

            //vScrollBar.Location = new Point(ClientRectangle.Width - 19, 3);
            //vScrollBar.Height = panel.Height;

            //hScrollBar.Location = new Point(3, ClientRectangle.Height - 19);
            //hScrollBar.Width = panel.Width;

            //hMax = (int)hScrollBar.Maximum - (int)hScrollBar.LargeChange + hScrollBar.SmallChange;
            //vMax = vScrollBar.Maximum - vScrollBar.LargeChange + vScrollBar.SmallChange;
        }       
    }
}