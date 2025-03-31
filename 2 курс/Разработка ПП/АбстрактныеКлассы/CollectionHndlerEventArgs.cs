namespace AbstractClasses
{
    public class CollectionHandlerEventArgs : EventArgs
    {
        public string ChangedCollection { get; set; }
        public string ChangeLog { get; set; }
        public Vehicle ChangedItem { get; set; }

        public override string ToString()
        {
            return $"{ChangedCollection} {ChangeLog} {ChangedItem}";
        }

        public CollectionHandlerEventArgs(string changedCollection, string changeLog, Vehicle changedItem)
        {
            this.ChangedCollection = changedCollection;
            this.ChangeLog = changeLog;
            this.ChangedItem = changedItem;
        }
    }
}