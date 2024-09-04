SELECT 
  list_aggregate(tau_pt, 'histogram'),
  list_aggregate(tau_eta, 'histogram'),
  list_aggregate(jet_pt, 'histogram'),
  list_aggregate(jet_eta, 'histogram'),
  list_aggregate(muon_pt, 'histogram'),
  list_aggregate(muon_eta, 'histogram'),
  list_aggregate(el_pt, 'histogram'),
  list_aggregate(el_eta, 'histogram'),
  list_aggregate(ph_pt, 'histogram'),
  list_aggregate(ph_eta, 'histogram')  
FROM singleMu;