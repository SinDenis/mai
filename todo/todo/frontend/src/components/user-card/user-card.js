import React from 'react';

import './user-card.css';

export default function UserCard(props) {
  return (
    <div className="card text-white bg-success d-flex align-items-center flex-row justify-content-between user-card">
      <h3 className="card-name">{ props.username }</h3>
      <div className="card-photo bg-danger"><span>{ props.username.charAt(0) }</span></div>
    </div>
  )
}
