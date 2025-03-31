using System;
using System.Collections;

namespace AbstractClasses
{
     public class Hash  : IEnumerable, ICloneable
    {
        protected ArrayList keys;
        protected List<Vehicle> values;
        int position = -1;

        public int CountElems  // Свойтво Count
        {
            get
            {
                return this.values.Count;
            }
        }

       protected int IndexOfKey(object key)
       {
            return this.keys.IndexOf(key);
       }

       public virtual Vehicle this[object key]  // Перегрузка индексирования
        {
            get 
            {
                return this.values[IndexOfKey(key)];
            }
            set
            {
                this.values[IndexOfKey(key)] = value;
            }
        }

        public Hash()  // Создание пустой коллекции
        {
            this.keys = new ArrayList();
            this.values = new List<Vehicle>();
        }

        public Hash(int capacity)  // Создание пустой коллекции с заданной емкостью
        {
            this.keys = new ArrayList(capacity);
            this.values = new List<Vehicle>(capacity);
        }

        public Hash(Hash h)  // Создание коллекции, инициализируемой элементами другой коллекции
        {
            this.keys = new ArrayList();
            this.values = new List<Vehicle>();
            for (int i = 0; i < h.CountElems; i++)
            {
                this.keys.Add(h.keys[i]);
                this.values.Add(h.values[i]);
            }
        }

        public virtual void AddElem(object key, Vehicle t)  // Добавление элемента
        {
            this.keys.Add(key);
            this.values.Add(t);
        }

        public virtual void AddRangeElems(Hash h)  // Добавление нескольких элементов
        {
            this.keys.AddRange(h.keys);
            this.values.AddRange(h.values);
        }

        public virtual void RemoveElem(object key)  // Тут надо как-то удалить элемент
        {
            int index = IndexOfKey(key);
            this.values.Remove(values[index]);
            this.keys.Remove(keys[index]);
        }

        public virtual void RemoveRangeElems(Hash h)  // Тут надо как-то удалить несколько элементов
        {
            int countElem = this.CountElems;
            for (int i = 0; i<h.CountElems; i++)
            {
                for (int j = 0; j<countElem; j++)
                {
                    if (this.keys[j] == h.keys[i])
                    {
                        this.keys.Remove(keys[j]);
                        this.values.Remove(values[j]);
                        break;
                    }
                }
            }
        }

        public virtual void ClearHash()  // Тут надо как-то удалить все элементы
        {
            this.keys = new ArrayList();
            this.values = new List<Vehicle>();
        }

        public virtual object FindElem(Vehicle value)  // Тут надо как-то найти элемент по значению
        {
            // сравнение через equels, перегрузить в классах 
            for (int i = 0; i < this.CountElems; i++)
            {
                if (this.values[i].Equals(value)) 
                {
                    return $"{this.values[i].Name}";
                }
            }
            return "Запрос не найден";
        }

        public virtual object Clone()
        {
            return new Hash(this);
        }

        public interface IEnumerable
        {
            IEnumerator GetEnumerator();
        }

        public IEnumerator GetEnumerator() => values.GetEnumerator();

        public bool MoveNext()
        {
            if (position < CountElems - 1)
            {
                position++;
                return true;
            }
            else
                return false;
        } // перемещение на одну позицию вперед в контейнере элементов

        public object Current
        {
            get
            {
                if (position == -1 || position >= CountElems)
                    throw new ArgumentException();
                return values[position];
            }
        }  // текущий элемент в контейнере

        public void Reset() => position = -1; // перемещение в начало контейнера
    }
}