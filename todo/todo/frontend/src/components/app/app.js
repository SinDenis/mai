import React, { Component } from 'react';
import {HashRouter, Route} from 'react-router-dom';
import './app.css';

import Register from '../register';
import Login from '../login';
import Header from '../header';
import Dashboard from '../dashboard';
import Raiting from '../raiting';
import MainDisplay from '../main-display';


export default class App extends Component {

  render() {
    return (
      <div>
        <HashRouter>
          <Header />
          <Route exact path="/" component={ MainDisplay } />
          <Route path="/dashboard/:username/:token" component={ Dashboard } />
          <Route path="/register" component={ Register } />
          <Route path="/login" component={ Login } />
          <Route path="/raiting" component={ Raiting } />
        </HashRouter>
      </div>
    )
  }

}
