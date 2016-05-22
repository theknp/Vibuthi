using System;
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
using Gat.Controls.Model;
using Gat.Controls.Framework;
using Gat.Controls;
using DicomImageLibrary;

namespace VibuthiMedicalImaging
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        int count = 1;
        List<CheckBoxListItem> Scans = new List<CheckBoxListItem>();
        OpenDialogViewModel odm;
        OpenDialogView od;

        Scan sc = null;
        DicomReader dd;
        List<byte> pixels8;
        List<ushort> pixels16;
        List<byte> pixels24; // 30 July 2010
        int imageWidth;
        int imageHeight;
        int bitDepth;
        int samplesPerPixel;  // Updated 30 July 2010
        bool imageOpened;
        double winCentre;
        double winWidth;
        bool signedImage;
        int maxPixelValue;    // Updated July 2012
        int minPixelValue;


        public MainWindow()
        {
          od = new OpenDialogView();
          odm = (Gat.Controls.OpenDialogViewModel)od.DataContext;
            odm.IsDirectoryChooser = true;
            
            odm.StartupLocation = WindowStartupLocation.CenterScreen;
            InitializeComponent();

            pixels8 = new List<byte>();
            pixels16 = new List<ushort>();
            pixels24 = new List<byte>();
            imageOpened = false;
            signedImage = false;
            maxPixelValue = 0;
            minPixelValue = 65535;
            //listScans.ItemsSource = Scans;
        }      

        private void btnLoadScan_Click(object sender, RoutedEventArgs e)
        {

            if (odm.Show() == true)
            {

                sc = new Scan(odm.SelectedFolder.Path.ToString());
                
                listScans.Items.Add(new CheckBoxListItem(true, odm.SelectedFolder.Name.ToString() + count));

                ReadAndDisplayDicomFile(0);
                //DicomImage.Source = sc.Images[0].getBitmap() as ImageSource;
                // MessageBox.Show(listScans.SelectedItems.Count.ToString());
                //listScans.ItemsSource = Scans;
            }
        }


        private void listScans_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            

        }

        private void scanItemCheckBox_Click(object sender, RoutedEventArgs e)
        {
            var cb = sender as CheckBox;
            var item = cb.DataContext as CheckBoxListItem;
            MessageBox.Show(item.Text + " " + item.Checked.ToString());
            MessageBox.Show(listScans.SelectedItems.Count.ToString());

        }

        private void ReadAndDisplayDicomFile(int index)
        {
            if (index >= 0 && index < sc.Images.Count)
            {

                dd = sc.Images[index];

                TypeOfDicomFile typeOfDicomFile = dd.typeofDicomFile;

                if (typeOfDicomFile == TypeOfDicomFile.Dicom3File ||
                    typeOfDicomFile == TypeOfDicomFile.DicomOldTypeFile)
                {
                    imageWidth = dd.width;
                    imageHeight = dd.height;
                    bitDepth = dd.bitsAllocated;
                    winCentre = dd.windowCentre;
                    winWidth = dd.windowWidth;
                    samplesPerPixel = dd.samplesPerPixel;
                    signedImage = dd.signedImage;

                    //label1.Visible = true;
                    //label2.Visible = true;
                    //label3.Visible = true;
                    //label4.Visible = true;
                    //bnSave.Enabled = true;
                    //bnTags.Enabled = true;
                    //bnResetWL.Enabled = true;
                    //label2.Text = imageWidth.ToString() + " X " + imageHeight.ToString();
                    //if (samplesPerPixel == 1)
                    //    label4.Text = bitDepth.ToString() + " bit";
                    //else
                    //    label4.Text = bitDepth.ToString() + " bit, " + samplesPerPixel +
                    //        " samples per pixel";

                    IPcontrol.NewImage = true;
                    //Text = "DICOM Image Viewer: " + fileNameOnly;

                    if (samplesPerPixel == 1 && bitDepth == 8)
                    {
                        pixels8.Clear();
                        pixels16.Clear();
                        pixels24.Clear();
                        dd.GetPixels8(ref pixels8);

                        // This is primarily for debugging purposes, 
                        //  to view the pixel values as ascii data.
                        //if (true)
                        //{
                        //    System.IO.StreamWriter file = new System.IO.StreamWriter(
                        //               "C:\\imageSigned.txt");

                        //    for (int ik = 0; ik < pixels8.Count; ++ik)
                        //        file.Write(pixels8[ik] + "  ");

                        //    file.Close();
                        //}

                        minPixelValue = pixels8.Min();
                        maxPixelValue = pixels8.Max();

                        // Bug fix dated 24 Aug 2013 - for proper window/level of signed images
                        // Thanks to Matias Montroull from Argentina for pointing this out.
                        if (dd.signedImage)
                        {
                            winCentre -= char.MinValue;
                        }

                        if (Math.Abs(winWidth) < 0.001)
                        {
                            winWidth = maxPixelValue - minPixelValue;
                        }

                        if ((winCentre == 0) ||
                            (minPixelValue > winCentre) || (maxPixelValue < winCentre))
                        {
                            winCentre = (maxPixelValue + minPixelValue) / 2;
                        }

                        IPcontrol.SetParameters(ref pixels8, imageWidth, imageHeight,
                            winWidth, winCentre, samplesPerPixel, true);//, this
                    }

                    if (samplesPerPixel == 1 && bitDepth == 16)
                    {
                        pixels16.Clear();
                        pixels8.Clear();
                        pixels24.Clear();
                        dd.GetPixels16(ref pixels16);

                        // This is primarily for debugging purposes, 
                        //  to view the pixel values as ascii data.
                        //if (true)
                        //{
                        //    System.IO.StreamWriter file = new System.IO.StreamWriter(
                        //               "C:\\imageSigned.txt");

                        //    for (int ik = 0; ik < pixels16.Count; ++ik)
                        //        file.Write(pixels16[ik] + "  ");

                        //    file.Close();
                        //}

                        minPixelValue = pixels16.Min();
                        maxPixelValue = pixels16.Max();

                        // Bug fix dated 24 Aug 2013 - for proper window/level of signed images
                        // Thanks to Matias Montroull from Argentina for pointing this out.
                        if (dd.signedImage)
                        {
                            winCentre -= short.MinValue;
                        }

                        if (Math.Abs(winWidth) < 0.001)
                        {
                            winWidth = maxPixelValue - minPixelValue;
                        }

                        if ((winCentre == 0) ||
                            (minPixelValue > winCentre) || (maxPixelValue < winCentre))
                        {
                            winCentre = (maxPixelValue + minPixelValue) / 2;
                        }

                        IPcontrol.Signed16Image = dd.signedImage;

                        IPcontrol.SetParameters(ref pixels16, imageWidth, imageHeight,
                            winWidth, winCentre, true);//, this
                    }

                    if (samplesPerPixel == 3 && bitDepth == 8)
                    {
                        // This is an RGB colour image
                        pixels8.Clear();
                        pixels16.Clear();
                        pixels24.Clear();
                        dd.GetPixels24(ref pixels24);

                        // This code segment is primarily for debugging purposes, 
                        //    to view the pixel values as ascii data.
                        //if (true)
                        //{
                        //    System.IO.StreamWriter file = new System.IO.StreamWriter(
                        //                      "C:\\image24.txt");

                        //    for (int ik = 0; ik < pixels24.Count; ++ik)
                        //        file.Write(pixels24[ik] + "  ");

                        //    file.Close();
                        //}

                        IPcontrol.SetParameters(ref pixels24, imageWidth, imageHeight,
                            winWidth, winCentre, samplesPerPixel, true);//, this
                    }
                }
                else
                {
                    if (typeOfDicomFile == TypeOfDicomFile.DicomUnknownTransferSyntax)
                    {
                        MessageBox.Show("Sorry, I can't read a DICOM file with this Transfer Syntax.",
                            "Warning", MessageBoxButton.OK, MessageBoxImage.Warning);
                    }
                    else
                    {
                        MessageBox.Show("Sorry, I can't open this file. " +
                            "This file does not appear to contain a DICOM image.",
                            "Error", MessageBoxButton.OK, MessageBoxImage.Error);
                    }

                    //Text = "DICOM Image Viewer: ";
                    // Show a plain grayscale image instead
                    pixels8.Clear();
                    pixels16.Clear();
                    pixels24.Clear();
                    samplesPerPixel = 1;

                    imageWidth = (int)IPcontrol.Width - 25;   // 25 is a magic number
                    imageHeight = (int)IPcontrol.Height - 25; // Same magic number
                    int iNoPix = imageWidth * imageHeight;

                    for (int i = 0; i < iNoPix; ++i)
                    {
                        pixels8.Add(240);// 240 is the grayvalue corresponding to the Control colour
                    }
                    winWidth = 256;
                    winCentre = 127;
                    IPcontrol.SetParameters(ref pixels8, imageWidth, imageHeight,
                        winWidth, winCentre, samplesPerPixel, true);//, this
                    IPcontrol.InvalidateVisual();

                    //label1.Visible = false;
                    //label2.Visible = false;
                    //label3.Visible = false;
                    //label4.Visible = false;
                    //bnSave.Enabled = false;
                    //bnTags.Enabled = false;
                    //bnResetWL.Enabled = false;
                }
            }
        }
    }
}
