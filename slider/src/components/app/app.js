import React, {Component} from 'react';
import './app.css';


import SliderItem from '../slider-item';
// import Loader from '../loader';
import Button from '../button';

export default class App extends Component {

  state = {
    load: false,
    idx: 0,
    pictures: []
  };

  getPictures = async (place) => {
    const url = 'https://picsum.photos/419/172/?random';
    let newPictures = [];
    for (let i = 0; i < 10; ++i) {
      let res = await fetch(url);
      let blobObj = await res.blob();
      let resURL =  await URL.createObjectURL(blobObj);
      newPictures.push(resURL);
    }
    this.setState(({idx, pictures, load}) => {
      if (place === 'end') {
        return {
          load: true,
          pictures: [...pictures, ...newPictures]
        };
      } else if (place === 'begin') {
        return {
          load: true,
          idx: 9,
          pictures: [...newPictures, ...pictures]
        };
      }
    });
  };

  next = () => {
    if (this.state.idx % 10 === 7) {
      this.setState(() => {
        return {
          load: false
        }
      });
      this.getPictures('end');
    }
    this.setState(({idx}) => {
      const newIdx = idx + 1;
      return {
        idx: newIdx
      };
    });
  };

  prev = () => {
    if (this.state.idx === 0) {
      this.setState(() => {
        return {
          load: false
        }
      });
      this.getPictures('begin');
      this.setState(() => {
        return {
          idx: 9
        };
      });
    } else {
      this.setState(({ idx }) => {
        const newIdx = idx - 1;
        return {
          idx: newIdx
        };
      });
    }
  };

  componentWillMount() {
    this.getPictures('end');
  }

  render() {
    return (
      <div className="app">
        <div className="slider-body">
          <SliderItem
            load={this.state.load}
            position={"left"}
            url={this.state.pictures[this.state.idx]}
          />
          <SliderItem
            load={this.state.load}
            url={this.state.pictures[this.state.idx + 1]}
          />
          <SliderItem
            load={this.state.load}
            position={"right"}
            url={this.state.pictures[this.state.idx + 2]}
          />
        </div>
        <div className="slider-buttons">
          <Button
            label="prev"
            btnClick={this.prev}
          />
          <Button
            label="next"
            btnClick={this.next}
          />
          {/*<Loader/>*/}
        </div>
      </div>
    );
  }
};
