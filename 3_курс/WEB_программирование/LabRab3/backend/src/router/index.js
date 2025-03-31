import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/Home.vue'
import ProductsView from '../views/Products.vue'
import ProductsTableView from '../views/ProductsTable.vue'
import AboutView from '../views/About.vue'
import DirectorView from '../views/Director.vue'
import MarketerView from '../views/Marketer.vue'
import WorkerView from '../views/Worker.vue'
import LoginView from '../views/Login.vue'
import RegistrationView from '../views/Registration.vue'
import AddProductView from "../views/AddProduct.vue"
import CartView from "../views/Cart.vue"


const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: HomeView
    },
    {
      path: '/about', 
      name: 'about',
      component: AboutView
    },
    {
      path: '/director',
      name: 'director',
      component: DirectorView
    },
    {
      path: '/marketer',
      name: 'marketer',
      component: MarketerView
    },
    {
      path: '/worker',
      name: 'worker',
      component: WorkerView
    },
    {
      path: '/products',
      name: 'products',
      component: ProductsView
    },
    {
      path: '/products_table',
      name: 'products_table',
      component: ProductsTableView
    },
    {
      path: '/add_product',
      name: 'add_product',
      component: AddProductView
    },
    {
      path: '/login',
      name: 'login',
      component: LoginView
    },
    {
      path: '/registration',
      name: 'registration',
      component: RegistrationView
    },
    {
      path: '/cart',
      name: 'cart',
      component: CartView
    },
  ]
})

export default router