using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VibuthiMedicalImaging
{
    public class CheckBoxListItem
    {
        public bool Checked { get; set; }
        public string Text { get; set; }

        public CheckBoxListItem(bool ch, string text)
        {
            Checked = ch;
            Text = text;
        }
    }
}
