using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Xml.Serialization;

namespace Carina.StateMachines
{
    public class Project
    {
        [XmlArray("TimeSlotList")] public List<TimeSlot> TimeSlot;
        public List<ModuleType> ModuleTypes;
        [XmlElement("ValueGroupBusList")] public BusValueList BusValues;
        [XmlElement("ValueGroupConstantList")] public ConstantValueList ConstantValues;
        
        private Dictionary<string, int> _busIdDictionary = null;
        [XmlIgnore]
        public Dictionary<string, int> BusID
        {
            get
            {
                _busIdDictionary ??= (from g in BusValues.ValueGroup
                    from v in g.ValueItem
                    orderby v.Index
                    select new
                    {
                        id = BusValues.ID + "." + g.ID + "." + v.ID,
                        index = v.Index
                    }).ToDictionary(p => p.id, v => v.index);
                
                return _busIdDictionary;
            }
        }

        [DebuggerDisplay("{value.ModuleType} {@group}.{value.ID}")]
        public struct BusValueIndexedItem
        {
            public string group;
            public BusValueItem value;
        }
        private List<BusValueIndexedItem> _busValuesIndexedList = null;

        [XmlIgnore]
        public List<BusValueIndexedItem> BusValuesIndexed
        {
            get
            {
                _busValuesIndexedList ??= (from g in BusValues.ValueGroup
                    from v in g.ValueItem
                    orderby v.Index
                    select new BusValueIndexedItem()
                    {
                        value = v,
                        @group = g.ID
                    }).ToList();
                
                return _busValuesIndexedList;
            }
        }
    }
}