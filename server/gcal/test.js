import dataStore from '../dataStore'
import { startEvent, endEvent } from './index'

// Sparkamus-Prime
const testCoreId = '54ff6f066672524845361167'

startEvent('primary', { duration: 15 }, testCoreId)
.then((eventId) => {
  dataStore[event.coreid] = {
    occupied: true,
    eventId
  }
}).catch((err) => {
  console.warn(err)
})

setTimeout(
  () => {
    endEvent('primary', { event: dataStore[testCoreId] }, testCoreId)
  }
, 5000)
