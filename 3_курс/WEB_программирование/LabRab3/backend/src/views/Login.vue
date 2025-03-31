<template>
  <div class="container d-flex justify-content-center align-items-center">
    <div class="card my-3">
      <div class="card-body">
        <h2 class="card-title">Вход</h2>
        <form @submit.prevent="login">
          <div class="mb-3">
            <label for="loginName" class="form-label">Имя пользователя:</label>
            <input type="text" id="loginName" v-model="name" class="form-control" required>
          </div>
          <div class="mb-3">
            <label for="loginPassword" class="form-label">Пароль:</label>
            <input type="password" id="loginPassword" v-model="password" class="form-control" required>
          </div>
          <div class="row">
              <button type="submit" class="btn btn-primary mx-3 col">Войти</button>
          </div>
          <div class="row">
              <router-link to="/registration" class="btn btn-info mx-3 mt-3 col">Регистрация</router-link>
          </div>
        </form>
      </div>
    </div>
  </div>
</template>

<script>
import Swal from 'sweetalert2';

export default {
  data() {
    return {
      name: '',
      password: '',
    };
  },
  methods: {
    login() {
      const loginData = {
        Name: this.name,
        Password: this.password,
      };

      fetch('http://localhost:1323/login', {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(loginData),
      })
        .then(response => {
          if (!response.ok) {
            throw new Error('Неверное имя пользователя или пароль');
          }
          return response.json();
        })
        .then(data => {
          console.log('Вход выполнен успешно', data);
          Swal.fire({
            icon: 'success',
            title: 'Вход выполнен успешно',
            showConfirmButton: true,
            timer: 1500,
          });

          localStorage.setItem('session-name', JSON.stringify({
            name: data.name,
            access: data.access,
          }));
          console.log('Пользователь: =', localStorage);
          this.$router.push('/');
        })

        .catch(error => {
          console.error('Ошибка входа:', error.message);
          Swal.fire({
            icon: 'error',
            title: 'Неверное имя пользователя или пароль',
            text: error.message
          });
        });
    },
  },
};
</script>