using System.Xml.Serialization;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;

namespace Carina.StateMachines
{
    [DebuggerDisplay("{ID}")]
    public class ModuleVariant
    {
        [XmlAttribute]
        public string ID;

        public List<LanguageElement> LanguageElementList;

        [XmlElement("GlobalValueList")] public ValueList GlobalValues;

        public List<StateMachine> StateMachineList;
        
        // Todo : add screens

        private Dictionary<string, int> _globalIdDictionary = null;
        [XmlIgnore]
        public Dictionary<string, int> GlobalID
        {
            get
            {
                _globalIdDictionary ??= (from g in GlobalValues.ValueGroup
                    from v in g.ValueItem
                    orderby v.Index
                    select new
                    {
                        id = GlobalValues.ID + "." + g.ID + "." + v.ID,
                        index = v.Index
                    }).ToDictionary(p => p.id, v => v.index);
                
                return _globalIdDictionary;
            }
        }
    }
}