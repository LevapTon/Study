<script setup>
</script>

<template>
  <header>
    <nav class="navbar navbar-expand-lg navbar-light bg-light">
      <div class="container-fluid">
          <a class="navbar-brand" href="/">
              <img src="assets/img/Schuhe-lineart.svg" alt="" width="30" height="24" class="d-inline-block align-text-top"> Ромашка</a>
              <button class="navbar-toggler" type="button"
              data-bs-toggle="collapse" data-bs-target="#navbarNav"
              aria-controls="navbarNav" aria-expanded="false" aria-label="Toggle navigation">
              <span class="navbar-toggler-icon"></span>
          </button>
          <div class="collapse navbar-collapse" id="navbarNav">
              <ul class="navbar-nav">
                  <li class="nav-item">
                    <router-link class="nav-link" to="/"><i class="bi-house"></i> Главная</router-link>
                  </li>
                  <li class="nav-item">
                    <router-link class="nav-link" to="/products"><i class="bi-basket"></i> Товары</router-link>
                  </li>
                  <li class="nav-item">
                    <router-link class="nav-link" to="/about"><i class="bi-info-square"></i> О нас</router-link>
                  </li>
              </ul>
          </div>
          <div class="collapse navbar-collapse navbar-nav justify-content-end">
            <li class="nav-item">
              <router-link class="nav-link" to="/cart"><i class="bi-cart-check"></i> Корзина</router-link>
            </li>
            <li class="nav-item">
              <router-link class="nav-link" to="/login"><i class="bi-person-square"></i> {{ sessionName }}</router-link>
            </li>
          </div>
      </div>
  </nav>
  </header>

  <main class="container">
    <RouterView /> <!--Именно в этом месте vue-router будет загружать компоненты в соответствие с навигацией-->
  </main>
  
  <footer>
  </footer>
</template>


<script>
export default {
  data() {
    return {
      sessionName: "",
    };
  },

  created() {
    const store = JSON.parse(localStorage.getItem('session-name'))
    if (store != undefined) {
      this.sessionName = store.name
    } else {
      this.sessionName = "Войти"
    }
  },
};
</script>

<style scoped>
</style>
