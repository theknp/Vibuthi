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

            if (odm.Show() == true)
            {

                Scan sc = new Scan(odm.SelectedFolder.Path.ToString());
                
                listScans.Items.Add(new CheckBoxListItem(true, odm.SelectedFolder.Path.ToString() + count));
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

       
    }
}
