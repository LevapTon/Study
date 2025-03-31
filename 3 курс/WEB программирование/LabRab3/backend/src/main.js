import 'vite/modulepreload-polyfill'
import { createApp } from 'vue'
import App from './App.vue'
import router from './router'

import * as bootstrap from '../public/assets/bootstrap/js/bootstrap.esm.js';

import './assets/main.css'
import '../public/assets/bootstrap/js/bootstrap.bundle.min.js'

const app = createApp(App)

app.use(router)

app.mount('#app')
