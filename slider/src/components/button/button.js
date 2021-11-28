import React from 'react';

import './button.css';

const Button = (props) => {
  return (
    <button onClick={props.btnClick} className="button" type="button">{props.label.toUpperCase()}</button>
  )
};

export default Button;
