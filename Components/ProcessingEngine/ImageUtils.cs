using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ProcessingEngine
{
    public class ImageUtils
    {
    }

    [DllImport(@"ImageProcInterface.dll", EntryPoint = "loadImageData", SetLastError = true)]
    public static extern int loadImageData(string imageFileName, ushort[] imageData, int numRows, int numCols);

}
