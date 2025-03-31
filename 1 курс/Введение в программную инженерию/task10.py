from brain import Brain

brain = Brain()
prompt = "Что Вас интересует?"

question = ""
while question != "хватит": 
    print(prompt, end = ' ')
    question = input()
    answer = brain.think(question)
    print(answer)