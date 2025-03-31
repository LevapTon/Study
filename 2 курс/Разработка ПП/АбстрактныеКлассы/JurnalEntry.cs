namespace AbstractClasses
{
    public class JournalEntry
    {
        public string CollectionName { get; set; }
        public string ChangeLog { get; set; }
        public string Publisher { get; set; }

        public override string ToString()
        {
            return $"{CollectionName}, {ChangeLog}, {Publisher}";
        }

        public JournalEntry(string collectionName, string changeLog, string publisher)
        {
            this.ChangeLog = changeLog;
            this.CollectionName = collectionName;
            this.Publisher = publisher;
        }
    }
}