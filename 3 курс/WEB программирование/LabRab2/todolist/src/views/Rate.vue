<template>
    <main class="container bg-secondary text-white">
      <!-- Декоративный элемент - горизонтальная линия -->
      <div class="decorative-line"></div>
  
      <div>
        <!-- Декоративный заголовок -->
        <h1 class="decorative-heading">Выберите оценку приложения:</h1>
        
        <div class="justify-content text-center">
            <div class="stars">
                <span
                  v-for="star in 5"
                  :key="star"
                  @click="setRating(star)"
                  :class="{ 'filled': star <= userRating }"
                >
                  &#9733; <!-- Звезда в виде символа Unicode -->
                </span>
                <h3>{{ getRatingMessage(userRating) }}</h3>
              </div>
        
              <div v-if="userRating !== null" class="result-message">
                <p>Ваша оценка: {{ userRating }} звезд{{ userRating !== 1 ? 'ы' : 'а' }}</p>
              </div>
        </div>
      </div>
    </main>
  </template>
  
  <script>
  export default {
    data() {
      return {
        userRating: null,
      };
    },
    methods: {
      setRating(rating) {
        this.userRating = rating;
      },
      getRatingMessage(rating) {
        switch (rating) {
          case 1:
            return 'Ужасно';
          case 2:
            return 'Плохо';
          case 3:
            return 'Удовлетворительно';
          case 4:
            return 'Хорошо';
          case 5:
            return 'Отлично';
          default:
            return '';
        }
      },
    },
  };
  </script>
  
  <style scoped>
  /* Стили для декоративного элемента - горизонтальной линии */
  .decorative-line {
    height: 10px;
    background-color: #f2f2f2;
    margin-bottom: 20px;
  }
  
  /* Стили для декоративного заголовка */
  .decorative-heading {
    font-size: 1.5em;
    text-align: center;
    margin-bottom: 10px;
  }
  
  .stars {
    font-size: 2em;
    cursor: pointer;
  }
  
  .filled {
    color: gold;
  }
  
  .result-message {
    margin-top: 20px;
  }
  </style>
  