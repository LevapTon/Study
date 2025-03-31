namespace AbstractClasses
{
    public class MyNewCollection : Hash
    {
        public delegate void CollectionHandler(object source, CollectionHandlerEventArgs args);
        public string Name { get; set; }

        public Journal Jour = new Journal();
              
        public event CollectionHandler CollectionCountChanged;
        
        public event CollectionHandler CollectionReferenceChanged;

        // Конструктор 1
        public MyNewCollection(string name): base()
        {
            this.Name = name;
            this.CollectionCountChanged += new CollectionHandler(this.Jour.OnCollectionCountChanged);
            this.CollectionReferenceChanged += new CollectionHandler(this.Jour.OnCollectionReferenceChanged);
        }

        // Конструктор 2
        public MyNewCollection(string name, int capacity): base(capacity)
        {
            this.Name = name;
            this.CollectionCountChanged += new CollectionHandler(this.Jour.OnCollectionCountChanged);
            this.CollectionReferenceChanged += new CollectionHandler(this.Jour.OnCollectionReferenceChanged);
        }

        // Конструктор 2
        public MyNewCollection(string name, Hash h): base(h)
        {
            this.Name = name;
            this.CollectionCountChanged += new CollectionHandler(this.Jour.OnCollectionCountChanged);
            this.CollectionReferenceChanged += new CollectionHandler(this.Jour.OnCollectionReferenceChanged);
            foreach (Vehicle v in h) OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "add", v));
        }

        // Обработчик события OnCollectionCountChanged
        public virtual void OnCollectionCountChanged(object source, CollectionHandlerEventArgs args)
        {
            if (CollectionCountChanged != null) CollectionCountChanged(source, args);
        }

        // Обработчик события OnCollectionReferenceChanged
        public virtual void OnCollectionReferenceChanged(object source, CollectionHandlerEventArgs args)
        {
            if (CollectionReferenceChanged != null) CollectionReferenceChanged(source, args);
        }

        // Добавление элемента        
        public override void AddElem(object? key, Vehicle t)
        {
            base.AddElem(key, t);
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "add", t));
        }

        // Добавление нескольких элементов
        public override void AddRangeElems(Hash h)
        {
            base.AddRangeElems(h);
            foreach (Vehicle v in h) OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "add", v));
        }
        
        // Удаление элемента
        public override void RemoveElem(object? key)
        { 
            OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "delete", this.values[IndexOfKey(key)]));
            base.RemoveElem(key);
        }

        // Удаление нескольких элементов
        public override void RemoveRangeElems(Hash h)
        {
            foreach (Vehicle v in h) OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "delete", v));
            base.RemoveRangeElems(h);
        }

        // Клонирование коллекции
        public override object Clone()
        {
            return new MyNewCollection("Second", this);
        }

        // Очистка коллекции
        public override void ClearHash()
        {
            foreach (Vehicle v in this) OnCollectionCountChanged(this, new CollectionHandlerEventArgs(this.Name, "delete", v));
            base.ClearHash();
        }

        // Поиск элемента по значению
        public override object FindElem(Vehicle value)
        {
            return base.FindElem(value);
        }

        // Индексатор
        public override Vehicle this[object key]
        {
            get
                {
                return base.values[IndexOfKey(key)];
                }
            set
            {
                OnCollectionReferenceChanged(this, new CollectionHandlerEventArgs(this.Name, "changed", this.values[IndexOfKey(key)]));
                base.values[IndexOfKey(key)] = value;
            }
        }
    }
}