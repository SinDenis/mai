import React, {Component} from 'react';

import Loader from '../loader';
import Heart from './icons/heart.svg';
import Bookmark from './icons/bookmark.svg';
import Share from './icons/share.svg';
import './slider-item.css';

export default class SliderItem extends Component {
  render() {

    const { load, position, url } = this.props;
    let picture;

    let sliderItemClasses = {
      main: 'slider-item',
      picture: 'slider-item-picture',
      options: 'slider-item-options',
    };

    if (position === 'left' || position === 'right') {
      for (let key in sliderItemClasses) {
        sliderItemClasses[key] += `__min`;
      }
      sliderItemClasses.main += ` slider-item-${position}`;
    }

    if (load) {
      picture = (
        <img
          className="slider-item-img"
          src={url}
          alt="#"
        />
      );
    } else {
      picture = (
        <Loader/>
      );
    }

    return (
      <div className={sliderItemClasses.main}>
        <div className={sliderItemClasses.picture}>
          {picture}
        </div>
        <div className={sliderItemClasses.options}>
          <img src={Heart} alt="heart"/>
          <img src={Bookmark} alt="bookmark"/>
          <img src={Share} alt="share"/>
        </div>
      </div>
    );
  }
}
