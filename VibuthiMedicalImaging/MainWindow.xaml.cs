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
        List<CheckBoxListItem> ScanList = new List<CheckBoxListItem>();
        OpenDialogViewModel odm;
        OpenDialogView od;
        
        List<Scan> LoadedScans = new List<Scan>();
        

        FusionAlgoCoordinator fusionAlgo;
        public MainWindow()
        {
          od = new OpenDialogView();
          odm = (Gat.Controls.OpenDialogViewModel)od.DataContext;
            odm.IsDirectoryChooser = true;
            
            odm.StartupLocation = WindowStartupLocation.CenterScreen;
            InitializeComponent();
           
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
                    Scan sc = null;
                    sc = new Scan(odm.SelectedFolder.Path.ToString());
                    //listScans.Items.Add(new CheckBoxListItem(true, odm.SelectedFolder.Name.ToString() + count));
                    listScans.Items.Add(odm.SelectedFolder.Name.ToString());

                    IDcontrol.CurrentScan = sc;
                    LoadedScans.Add(sc);
                }
                //DicomImage.Source = sc.Images[0].getBitmap() as ImageSource;
                // MessageBox.Show(listScans.SelectedItems.Count.ToString());
                //listScans.ItemsSource = Scans;
            }
        }


        private void listScans_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var cb = sender as ListBox;
            //Before cahnging the scan get the current index of displayed scan

            int sourceimgIndex = IDcontrol.CurrentScan.CurrentImageIndex;

            if (cb.SelectedIndex < LoadedScans.Count)
            {
                int imageIndex = 0;
                if (SynchronizeScans)
                {
                    Boolean isRefScan = cb.SelectedIndex == 0;
                    AlignInfo aInfo = fusionAlgo.GetAlginData(sourceimgIndex, !isRefScan);
                    if (isRefScan)
                        imageIndex = aInfo.parentImageIndex;
                    else
                        imageIndex = aInfo.matchingImageIndex;
                }
                else
                {
                    imageIndex = sourceimgIndex;

                    if (imageIndex >= LoadedScans[cb.SelectedIndex].Images.Count)
                        imageIndex = LoadedScans[cb.SelectedIndex].Images.Count - 1;

                }
                LoadedScans[cb.SelectedIndex].CurrentImageIndex = imageIndex;
                IDcontrol.CurrentScan = LoadedScans[cb.SelectedIndex];
            }
            else
            {
                MessageBox.Show(" Selected Index is  " + cb.SelectedIndex.ToString() + " Load Count is " + LoadedScans.Count.ToString());
            }            
        }

        private void scanItemCheckBox_Click(object sender, RoutedEventArgs e)
        {
            var cb = sender as CheckBox;
            var item = cb.DataContext as CheckBoxListItem;
            MessageBox.Show(item.Text + " " + item.Checked.ToString());
            MessageBox.Show(listScans.SelectedItems.Count.ToString());

        }

        //private void btnReconstruct_Click(object sender, RoutedEventArgs e)
        //{
        //    ComputeAlignOffsets(sc.Images[0], sc.Images[1]);
        //}

        //private void ComputeAlignOffsets(DicomReader dicomReader1, DicomReader dicomReader2)
        //{
        //    List<ushort> pixels1 = new List<ushort>();
        //    List<ushort> pixels2 = new List<ushort>();

        //    dicomReader1.GetPixels16(ref pixels1);
        //    dicomReader2.GetPixels16(ref pixels2);
        //    int width = dicomReader1.width;
        //    int height = dicomReader1.width;


        //    ImageUtils util = new ImageUtils();
        //    List<double> corrImage = new List<double>();
        //    //util.matchImages(pixels1.ToArray(), width, height, pixels2, width, height, corrImage);
        //}

        private void FuseScans_Click(object sender, RoutedEventArgs e)
        {
            if(LoadedScans.Count == 2)
            {
                fusionAlgo = new FusionAlgoCoordinator();
                fusionAlgo.PerformFusion(ref LoadedScans);
                SynchronizeScans = true;
                updateButtonStatus();
            }
            else
            {
                MessageBox.Show("Load 2 Scans for Fusion!");
            }
        }
        public Boolean SynchronizeScans = false;
        private void SyncScans_Click(object sender, RoutedEventArgs e)
        {
            SynchronizeScans = !SynchronizeScans;
            updateButtonStatus();
        }
        public void updateButtonStatus()
        {             
            if(SynchronizeScans)
            {
                SyncScans.Content = "ReleaseScans";
            }
            else
            {
                SyncScans.Content = "SyncScans";
            }
        }
    }
}
