<script setup>
</script>

<template>
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Главная страница</title>
  </head>


  <main class="container">
    <h1>Обувной магазин "Ромашка"</h1>
    <p>Добро пожаловать!<br>Мы подбираем для Вас лучшую обувь для любого сезона!</p>

    <div class="container d-flex justify-content-center" style="max-width: 40%; max-height: 30%;">
      <div id="carouselWithIndicators" class="carousel slide pointer-event" data-bs-ride="carousel">
        <ol class="carousel-indicators">
            <li data-bs-target="#carouselWithIndicators" data-bs-slide-to="0" class="active"></li>
            <li data-bs-target="#carouselWithIndicators" data-bs-slide-to="1"></li>
            <li data-bs-target="#carouselWithIndicators" data-bs-slide-to="2"></li>
        </ol>
        <div class="carousel-inner">
            <div class="carousel-item active">
                <img src="../../public/assets/img/main_1.jpg" class="" alt="Slide 1">
            </div>
            <div class="carousel-item">
                <img src="../../public/assets/img/main_2.jpg" class="" alt="Slide 2">
            </div>
            <div class="carousel-item">
                <img src="../../public/assets/img/main_3.jpg" class="" alt="Slide 3">
            </div>
        </div>
        <a class="carousel-control-prev" href="#carouselWithIndicators" role="button" data-bs-slide="prev">
            <span class="carousel-control-prev-icon" aria-hidden="true"></span>
            <span class="visually-hidden">Previous</span>
        </a>
        <a class="carousel-control-next" href="#carouselWithIndicators" role="button" data-bs-slide="next">
            <span class="carousel-control-next-icon" aria-hidden="true"></span>
            <span class="visually-hidden">Next</span>
        </a>
      </div>
    </div>

    <h2><br>Отзывы о нас</h2>
    <div>
        <table class="table">
            <tr>
                <th>Имя</th>
                <th>Отзыв</th>
                <th>Рейтинг</th>
            </tr>
            <tr>
                <td>Влад</td>
                <td>Хорошая компания! Довольно качественный продукт.</td>
                <td>5</td>
            </tr>
            <tr>
                <td>Оксана</td>
                <td>Обслуживание хорошее, но, к сожалению, обувь расклеилась через месяц</td>
                <td>3</td>
            </tr>
            <tr>
                <td>Игорь</td>
                <td>Брал кросовки для бега. Покупкой полностью доволен.</td>
                <td>5</td>
            </tr>
        </table>
    </div>
  </main>
  
</template>

<script>
import"../../public/assets/bootstrap/js/bootstrap.bundle.min.js"

export default {
  data() {
    return {
    };
  },
  mounted() {
      new bootstrap.Carousel(document.getElementById('carouselWithIndicators'), {
        interval: 3000,
      });
  },

  methods: {
    async testPost() {
      const response = await fetch('/testpost', {
        method: 'POST', 
        headers: {
          'Content-Type': 'application/json'
        },
        body: JSON.stringify({
          name: "Иван"
        }) 
      });

      const jsonResult = await response.json();

      console.log(jsonResult);
    },
    async testGet() {
      // Отправляем запрос типа GET
      const response = await fetch(`/testget?name=Василий`, {
        method: 'GET'
      });

      const textResult = await response.text();

      console.log(textResult);
    }
  }
}
</script>
