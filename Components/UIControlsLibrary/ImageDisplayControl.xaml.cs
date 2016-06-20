using DicomImageLibrary;
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

namespace UIControlsLibrary
{
    /// <summary>
    /// Interaction logic for ImageDisplayControl.xaml
    /// </summary>
    public partial class ImageDisplayControl : UserControl
    {
        public ImageDisplayControl()
        {
            InitializeComponent();
        }
        Scan currentScan = null;       
             
        //public ViewSettings viewSettings;
        public bool viewSettingsChanged;
        public Scan CurrentScan
        {
            get
            {
                return currentScan;
            }
            set
            {
                currentScan = value;
                IPControl.PanelType = 1;
                IPControl.CurrentScan = currentScan;

                IPControlLeft.PanelType = 2;
                IPControlLeft.CurrentScan = currentScan;

                IPControlRight.PanelType = 3;
                IPControlRight.CurrentScan = currentScan;
            }
        }

        public byte[] ImageLeftBytes
        {
            set
            {
                byte[] array = value;
                using (System.IO.MemoryStream ms = new System.IO.MemoryStream(array))
                {
                    BitmapImage image = new BitmapImage();
                    image.BeginInit();
                    image.StreamSource = ms;
                    image.EndInit();
                    ImageLeft = image;
                }
            }
        }
        public byte[] ImageRightBytes
        {
            set
            {
                byte[] array = value;
                using (System.IO.MemoryStream ms = new System.IO.MemoryStream(array))
                {
                    BitmapImage image = new BitmapImage();                    
                    image.BeginInit();
                    image.StreamSource = ms;
                    image.EndInit();
                    ImageRight = image;
                }
            }
        }
        public byte[] ImageCentreBytes
        {
            set
            {
                byte[] array = value;
                using (System.IO.MemoryStream ms = new System.IO.MemoryStream(array))
                {
                    BitmapImage image = new BitmapImage();
                    image.BeginInit();
                    image.StreamSource = ms;
                    image.EndInit();
           //         ImageCentre = image;
                }
            }
        }
        
        //public Scan CurrentScan
        //{

        //}
        private BitmapImage ImageLeft
        {
            get
            {
                return imageLeft.Source as BitmapImage;
            }
            set
            {
                imageLeft.Source = value;
            }
        }

        private BitmapImage ImageRight
        {
            get
            {
                return imageRight.Source as BitmapImage;
            }
            set
            {
                imageRight.Source = value;
                
            }
        }

        //private BitmapImage ImageCentre
        //{
        //    get
        //    {
        //        return imageCentre.Source as BitmapImage;
               
        //    }
        //    set
        //    {
        //        imageCentre.Source = value;
        //        centreLines();
        //    }
        //}

        int pixLocX;
        int pixLocY;
        public int PixLocX
        {
            get
            {
                return pixLocX;
            }
            private set
            {
                pixLocX = value;
                LocationChanged(this, EventArgs.Empty);
            }
        }
        public int PixLocY
        {
            get
            {
                return pixLocY;
            }

            private set
            {
                pixLocY = value;
                LocationChanged(this, EventArgs.Empty);
            }
        }
        public event LocationChangedEventHandler LocationChanged;
        public void centreLines()
        {
           //lineH.X1 = imageCentre.
        }
        public delegate void LocationChangedEventHandler(object sender, EventArgs e);
        private void sliderHorix_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //PixLocX = (int)(imageCentre.Source.Width * e.NewValue);
            //txtBlock.Text = sliderHorix.Value.ToString() + " X " + sliderVertic.Value.ToString();
        }

        private void sliderVertic_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            //PixLocY = (int)(imageCentre.Source.Height * e.NewValue);
            //txtBlock.Text = sliderHorix.Value.ToString() + " X " + sliderVertic.Value.ToString();
        }
    }
}
