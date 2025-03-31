namespace AbstractClasses
{
    public class Journal
    {
        private List<JournalEntry> _entries = new List<JournalEntry>();

        public void OnCollectionCountChanged(object source, CollectionHandlerEventArgs args)
        {
            _entries.Add(new JournalEntry(args.ChangedCollection, args.ChangeLog, args.ChangedItem.Name));
        }
        
        public void OnCollectionReferenceChanged(object source, CollectionHandlerEventArgs args)
        {
            _entries.Add(new JournalEntry(args.ChangedCollection, args.ChangeLog, args.ChangedItem.Name));
        }

        public override string ToString()
        {
            string str = "";
            foreach (var entry in _entries)
            {
                str += entry;
                str += "\n";
            }
            return str;
        }
    }
}