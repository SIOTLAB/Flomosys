/**
 * Initializes the Vue components
 *
 * @author: Tai Groot
 */
function showUI() {
  deviceRegistry = new Vue({
    el: '#locations',
    data() {
      return {
        transProps: {
          name: 'flip-list',
        },
        locations: getDevices,
        streets: {
          'Location': {
            key: 'location',
          },
        },
      };
    },
    methods: {
    },
  });
  detailTable = new Vue({
    el: '#metrics',
    data() {
      return {
        transProps: {
          name: 'flip-list',
        },
        dateSort: false,
        metrics: fillData,
        fields: {
          'Date': {
            sortBy: 'timestamp',
          },
          'Value': {
            key: 'value',
          },
        },
      };
    },
    methods() {

    },
  });
}
/**
 * Fetches the device list (mocked up for now, returns a constant list)
 *
 * @return {array} Array of device objects
 * @author: Tai Groot
 */
function getDevices() {
  let devices = [];
  // get deivce list here later, right now we have one
  devices = [{'location': 'E Williams St'}];
  return devices;
}
/**
 * Fetches the metrics from the endpoint and loads as JSON
 *
 * @author: Tai Groot
 * @return {array} Array of metric objects
 */
async function getData() {
  const items= await fetch('https://flomosys.com/data.json').then(function(data) {
    return data.json();
  });
  return items;
}
/**
 * Formats collected data correctly for the table
 *
 * @param {Object} ctx Vue context object
 * @param {function} callback Vue callback to fill the table data
 * @author: Tai Groot
 */
function fillData(ctx, callback) {
  const options = {year: 'numeric',
    month: 'numeric',
    day: 'numeric',
    hour: 'numeric',
    minute: 'numeric',
    second: 'numeric',
  };

  getData().then(function(items) {
    items.forEach(function(item, index) {
      const date = new Date(item.timestamp);
      item.Date = date.toLocaleDateString('en-US', options);
    });
    callback(items);
  });
}
showUI();
