using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DicomImageLibrary
{
    public class Scan
    {
        ScanInfo info;
        List<DicomReader> dicomImages = new List<DicomReader>();

        public Scan(string path)
        {
            String[] files = System.IO.Directory.GetFiles(path);
            //dicomImages = new DicomReader[files.Length];

            Parallel.ForEach(files, (currentFile) =>
            {
                DicomReader dr = new DicomReader();
                dr.DicomFileName = currentFile;
                dicomImages.Add(dr);

            });

        }
        public List<DicomReader> Images { get { return dicomImages; } }
    }
}
