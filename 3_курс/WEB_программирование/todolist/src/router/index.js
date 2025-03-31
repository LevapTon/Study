import { createRouter, createWebHistory } from 'vue-router'
/* Импорт компонента главной страницы */
import TodoView from '../views/Todo.vue'
/* Импорт компонента страницы "Об авторе" */
import AboutView from '../views/About.vue'
/* Импорт компонента страницы "Оценить" */
import RateView from '../views/Rate.vue'


const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
  {
    path: '/', /* Здесь задается путь в адресной строке для
    страницы, для главной - это корневой путь / */
    name: 'todo', /* Здесь задается наименование маршрута */
    component: TodoView /* Здесь задается компонент для
    маршрута, указываем TodoView - главная страница */
  },
  {
    path: '/about', /* Путь для страницы "Об авторе" - /about */
    name: 'about',
    component: AboutView /* Компонент для страницы "Об авторе" -
    AboutView */
  },
  {
    path: '/rate', /* Путь для страницы "Оценить" - /rate */
    name: 'rate',
    component: RateView /* Компонент для страницы "Оценить" -
    RateView */
  },
  ]
})


export default router