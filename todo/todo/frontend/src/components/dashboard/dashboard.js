import React, {Component} from 'react';

import './dashboard.css';

import Todo from '../todo';
import UserCard from '../user-card';

export default class Dashboard extends Component {

  render() {
    return (
      <div>
        <UserCard username={ this.props.match.params.username }/>
        <Todo token={ this.props.match.params.token }/>
      </div>
    )
  }
  
}