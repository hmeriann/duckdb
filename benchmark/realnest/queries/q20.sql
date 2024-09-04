SELECT 
  list_aggregate(tau_pt, 'last'),
  list_aggregate(tau_eta, 'last'),
  list_aggregate(jet_pt, 'last'),
  list_aggregate(jet_eta, 'last'),
  list_aggregate(muon_pt, 'last'),
  list_aggregate(muon_eta, 'last'),
  list_aggregate(el_pt, 'last'),
  list_aggregate(el_eta, 'last'),
  list_aggregate(ph_pt, 'last'),
  list_aggregate(ph_eta, 'last')  
FROM singleMu;