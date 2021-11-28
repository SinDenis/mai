import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { BrowserRouter as Router } from "react-router-dom";

import App from './components/app';
import ErrorBoundry from './components/error-boundry';
import BookstoreService from './services/bookstore-service';
import { BookStoreServiceProvider } from "./components/bookstore-service-context";

import store from './store';

const bookstoreService = new BookstoreService();

ReactDOM.render(
  <Provider store={ store }>
    <ErrorBoundry>
      <BookStoreServiceProvider value={ bookstoreService }>
        <Router>
          <App />
        </Router>
      </BookStoreServiceProvider>
    </ErrorBoundry>
  </Provider>,
  document.getElementById('root')
);
