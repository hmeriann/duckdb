SELECT 
  list_aggregate(tau_pt, 'stddev_pop'),
  list_aggregate(tau_eta, 'stddev_pop'),
  list_aggregate(jet_pt, 'stddev_pop'),
  list_aggregate(jet_eta, 'stddev_pop'),
  list_aggregate(muon_pt, 'stddev_pop'),
  list_aggregate(muon_eta, 'stddev_pop'),
  list_aggregate(el_pt, 'stddev_pop'),
  list_aggregate(el_eta, 'stddev_pop'),
  list_aggregate(ph_pt, 'stddev_pop'),
  list_aggregate(ph_eta, 'stddev_pop')  
FROM singleMu;