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
using ImageProcWrapper;

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
            //ImageUtils util = new ImageUtils();
           // util.method(10, 20);
            
            if (odm.Show() == true)
            {

                if (odm.SelectedFolder != null)
                {
                    sc = new Scan(odm.SelectedFolder.Path.ToString());
                    listScans.Items.Add(new CheckBoxListItem(true, odm.SelectedFolder.Name.ToString() + count));

                    IPcontrol.CurrentScan = sc;
                    //ReadAndDisplayDicomFile(0);
                }
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

        private void btnReconstruct_Click(object sender, RoutedEventArgs e)
        {
            ComputeAlignOffsets(sc.Images[0], sc.Images[1]);
        }

        private void ComputeAlignOffsets(DicomReader dicomReader1, DicomReader dicomReader2)
        {
            List<ushort> pixels1 = new List<ushort>();
            List<ushort> pixels2 = new List<ushort>();

            dicomReader1.GetPixels16(ref pixels1);
            dicomReader2.GetPixels16(ref pixels2);
            int width = dicomReader1.width;
            int height = dicomReader1.width;


            ImageUtils util = new ImageUtils();
            List<double> corrImage = new List<double>();
            //util.matchImages(pixels1.ToArray(), width, height, pixels2, width, height, corrImage);
        }
    }
}
