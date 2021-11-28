
const compose = ( ...functions ) => (component) => {
  return functions.reduceRight((previousValue, f) => f(previousValue), component)
};

export default compose;