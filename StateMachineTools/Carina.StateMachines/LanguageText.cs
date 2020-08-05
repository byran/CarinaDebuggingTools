using System.Diagnostics;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    [DebuggerDisplay("'{Language}' {Text}")]
    public class LanguageText
    {
        [XmlAttribute] public string Language;

        [XmlText] public string Text;
    }
}