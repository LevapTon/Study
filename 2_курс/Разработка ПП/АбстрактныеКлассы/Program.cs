using System;
using System.Collections.Generic;
using static AbstractClasses.MyNewCollection;

namespace AbstractClasses
{
    class Program
    {
        static void Main(string[] args)
        {
            List<Vehicle> vehicles = new List<Vehicle>()
            {
                new Plane("Гражданские самолёты Сухого", "Sukhoi Superjet 100", 2007, 108, 2167920000),
                new Plane("Airbus S.A.S.", "Airbus A380", 2005, 525, 29447580000), 
                new Plane("Boeing", "Boeing 787 Dreamliner", 2009, 250, 14952626000),
                new Plane("Airbus", "Airbus A321", 1994 , 185, 5660680000),
                new Plane("КБ Ильюшина", "Ил-96", 1988 , 435, 85000000000),
                new Car("Ульяновский автозавод", "УАЗ-3163 «Patriot»", 2005, 5, 1188400),
                new Car("GM Daewoo", "Chevrolet Lacetti", 2002, 5, 400000), 
                new Car("КАМАЗ", "KAMAZ-5490", 2013, 3, 3500000),
                new Car("Daimler AG", "Mercedes-Benz S-Class (C217)", 2015 , 2, 6950000),
                new Car("Горьковский автозавод", "ГАЗель NEXT A65R52-80", 1988 , 22, 4500000),
                new Ship("ÖSWAG", "NAZCA", "Москва", 2020, 8, 275000000),
                new Ship("M/Y MIA", "Monte Carlo Yachts", "Слима", 2017, 31, 338554000), 
                new Ship("Azimut 43S", "Azimut", "Москва", 2014, 4, 35000000),
                new Ship("Riva 88' Domino Super", "Riva", "Рим", 2016 , 20, 267100000),
                new Ship("Bavaria R40 Coupe", "Bavaria", "", 2017 , 12, 23326060),
            };

            MyNewCollection hOne = new MyNewCollection("First");
            vehicles.Sort();
            for (int i = 0; i < vehicles.Count; i++)
            {
                //hOne[vehicles[i].Name] = vehicles[i];
            
                hOne.AddElem(vehicles[i].Name, vehicles[i]);
            }
            foreach(Vehicle item in hOne)
                Console.WriteLine(item.PrintVehicle());
            
            MyNewCollection hTwo = new MyNewCollection("Second");
            hTwo = (MyNewCollection)hOne.Clone();
            
            Console.WriteLine('\n' + "Измененная таблица");
            hOne.RemoveElem("Chevrolet Lacetti");
            hOne.RemoveElem("УАЗ-3163 «Patriot»");
            hOne.RemoveElem("KAMAZ-5490");
            hOne.RemoveElem("Mercedes-Benz S-Class (C217)");
            hOne.RemoveElem("ГАЗель NEXT A65R52-80");

            foreach(Vehicle item in hOne)
                Console.WriteLine(item.PrintVehicle());
            

            Console.WriteLine('\n' + "Скопированная таблица");

            foreach(Vehicle item in hTwo)
                Console.WriteLine(item.PrintVehicle());
            

            Console.WriteLine('\n' + "Удаление диапозона");

            hTwo.RemoveRangeElems(hOne);

            foreach(Vehicle item in hTwo)
                Console.WriteLine(item.PrintVehicle());
            

            Console.WriteLine('\n' + "Добавление диапозона");

            hOne.AddRangeElems(hTwo);

            foreach(Vehicle item in hOne)
                Console.WriteLine(item.PrintVehicle());
            

            Console.WriteLine('\n' + "Поиск");
            Console.WriteLine(vehicles[3].PrintVehicle());
            Console.WriteLine(hOne.FindElem(vehicles[3]));

            Console.WriteLine('\n' + "Изменение по индексу");
            Console.WriteLine(hOne["KAMAZ-5490"].PrintVehicle());
            hOne["KAMAZ-5490"] = new Car("АвтоВАЗ", "ВАЗ-2121 «Ни́ва»", 1977, 4, 859900);
            Console.WriteLine(hOne["KAMAZ-5490"].PrintVehicle());

            Console.WriteLine('\n' + "Журнал 1");
            Console.WriteLine(hOne.Jour);
            Console.WriteLine('\n' + "Журнал 2");
            Console.WriteLine(hTwo.Jour);
        }
    }
}