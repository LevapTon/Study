class Player():
    isHandsEmpty = True
    isHaveAKey = False
    currentRoom = 2


class Game():
    def showMap():
        print(" "+"-"*7)
        print("←"+"|1|↔|2|")
        print(" -----↕-")
        print(" |4|↔|3|")
        print(" "+"-"*7)

    
    isDoor2Unlock = False
    isDoor3Unlock = False
    keyOnAFloor = True
    isLeverPooled = False
    isPlatePushed = False
    kettlebellCounter = 2


if __name__ == "__main__":
    print("\nВаша задача: открыть дверь в комнате 1 и покинуть здание.")
    print("Вы начинаете в комнате 2. Ниже представлена карта здания.")
    print("Удачи!\n")
    Game.showMap()

    gameIsDone = False

    while not gameIsDone:
        print("\nВы в комнате №%d" % Player.currentRoom)
        match Player.currentRoom:
            # Действия в первой комнате
            case 1:
                print("Что вы будете делать?")
                print("0 – Посмотреть на карту")
                print("1 – Покинуть здание")
                print("2 – Пойти в комнату №2")
                if Game.kettlebellCounter != 0:
                    print("3 – Взять гирю")
                choice = int(input("Ваш выбор: "))
                match choice:
                    # Просмотр карты
                    case 0:
                        Game.showMap()
                    # Выход из здания
                    case 1:
                        if not Game.isPlatePushed:
                            print("\nДверь закрыта. Нет ни дверной ручки, ни каких-либо замков.")
                            print("Возможно эта дверь открывается как-то иначе.")
                        else:
                            gameIsDone = not gameIsDone
                    # Переход в комнату 2
                    case 2:
                        Player.currentRoom = 2
                    # Поднятие гири
                    case 3:
                        if Player.isHandsEmpty:
                            print("\nВы подняли гирю")
                            Player.isHandsEmpty = not Player.isHandsEmpty
                            Game.kettlebellCounter -= 1
                        else:
                            print("\nРуки уже заняты.")
            # Действия во второй комнате
            case 2:
                print("Что вы будете делать?")
                print("0 – Посмотреть на карту")
                print("1 – Пойти в комнату №1")
                print("2 – Пойти в комнату №3")
                print("3 – Дернуть за рычаг")
                choice = int(input("Ваш выбор: "))
                match choice:
                    # Просмотр карты
                    case 0:
                        Game.showMap()
                    # Переход в комнату 1
                    case 1:
                        if not Game.isDoor2Unlock:
                            if Player.isHaveAKey:
                                print("\nВы открыли дверь ключом.")
                                Game.isDoor2Unlock = not Game.isDoor2Unlock
                                Player.currentRoom = 1
                            else:
                                print("\nДверь закрыта. На ней висит замок. Возможно где-то есть ключ?")
                        else:
                            Player.currentRoom = 1
                    # Переход в комнату 3
                    case 2:
                        Player.currentRoom = 3
                    # Действия с рычагом
                    case 3:
                        if not Game.isLeverPooled:
                            print("\nРычаг переключился и сломался. Где-то открылась дверь.")
                            Game.isLeverPooled = not Game.isLeverPooled
                            Game.isDoor3Unlock = not Game.isDoor3Unlock
                        else:
                            print("\nРычаг сломан, от него уже нет толку.")
            # Действия в третьей комнате
            case 3:
                if Game.keyOnAFloor:
                    print("\nНа полу лежит ключ. Вы подобрали его.")
                    Player.isHaveAKey = not Player.isHaveAKey
                    Game.keyOnAFloor = not Game.keyOnAFloor
                print("Что вы будете делать?")
                print("0 – Посмотреть на карту")
                print("1 – Пойти в комнату №4")
                print("2 – Пойти в комнату №2")
                choice = int(input("Ваш выбор: "))
                match choice:
                    # Просмотр карты
                    case 0:
                        Game.showMap()
                    # Переход в комнату 4
                    case 1:
                        if not Game.isDoor3Unlock:
                            print("\nДверь закрыта. Нет ни дверной ручки, ни каких-либо замков.")
                            print("Возможно эта дверь открывается как-то иначе.")
                        else:
                            Player.currentRoom = 4
                    # Переход в комнату 2
                    case 2:
                        Player.currentRoom = 2
            # Действия в четвертой комнате
            case 4:
                if not Game.isPlatePushed:
                    print("\nНа полу видна нажимная плита.")
                    print("Возможно она что-то откроет, если достаточно надавить на нее?")
                print("Что вы будете делать?")
                print("0 – Посмотреть на карту")
                print("1 – Пойти в комнату №3")
                if not Player.isHandsEmpty:
                    print("2 – Поставить гирю на плиту")
                choice = int(input("Ваш выбор: "))
                match choice:
                    # Просмотр карты
                    case 0:
                        Game.showMap()
                    # Выход из здания
                    case 1:
                        Player.currentRoom = 3
                    # Поставить гирю
                    case 2:
                        if Game.kettlebellCounter != 0:
                            print("\nВы поставили гирю. Видимо, веса не достаточно.")
                            Player.isHandsEmpty = not Player.isHandsEmpty
                        else:
                            print("\nВы поставили гирю. Где-то открылась дверь.")
                            Player.isHandsEmpty = not Player.isHandsEmpty
                            Game.isPlatePushed = not Game.isPlatePushed
    print("\nПоздравляем! Вы завершили игру!")
